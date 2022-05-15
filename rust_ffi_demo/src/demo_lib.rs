use std::ffi::{CStr, CString};
use std::os::raw::c_char;

// TODO: don't just make every field public

#[derive(Debug)]
#[repr(C)]
enum FFI_DL_EventType {
    Created,
    Destroyed,
    Changed,
}

#[repr(C)]
struct FFI_DL_Event {
    pub event_type: FFI_DL_EventType,
}

#[repr(C)]
struct FFI_DL_CreatedEvent {
    pub baseEvent: FFI_DL_Event,
    pub creationString: *const c_char,
}

#[repr(C)]
struct FFI_DL_DestroyedEvent {
    pub baseEvent: FFI_DL_Event,
    pub destroyedByte: u8,
}

#[derive(Debug, Clone, Copy)]
#[repr(C)]
pub enum FFI_ChangedState {
    A,
    B,
}

struct FFI_DL_ChangedEvent {
    pub baseEvent: FFI_DL_Event,
    pub changedState: FFI_ChangedState,
}

type FFI_DL_EventList = *const *const FFI_DL_Event;

#[link(name = "demo_lib_static", kind = "static")]
extern "C" {
    fn DL_GetEvents(
        /* Out */ count: *mut u32,
        /* Out */ events: *mut FFI_DL_EventList,
    ) -> i32;

    fn DL_ReturnEvents(count: u32, events: FFI_DL_EventList) -> i32;

    fn DL_HandleCreatedEvent(creationString: *const c_char) -> i32;

    fn DL_HandleDestroyedEvent(destroyedByte: u8) -> i32;

    fn DL_HandleChangedEvent(changedState: ChangedState) -> i32;
}

// The FFI_ChangedState enum is fine to present in our safe interface, but let's alias away that
// gnarly FFI name
pub type ChangedState = FFI_ChangedState;

// FIXME: is there anyway to just present a rust-safe view of things without copying? I feel like
// this is a job for slices but idk
#[derive(Debug)]
pub enum DlEvent {
    // FIXME: avoid a string allocation+copy
    CreatedEvent { creationString: CString },
    DestroyedEvent { destroyedByte: u8 },
    ChangedEvent { changedState: ChangedState },
}

// TODO: maybe on drop(..) this should call ReturnEvents or something. Then the lifetime of the events could be tied to the lifetime of the structure which would be interesting
pub struct DlEventList {
    pub events: Vec<DlEvent>,
    return_ptr: FFI_DL_EventList,
}

pub fn get_events() -> DlEventList {
    unsafe {
        let mut count: u32 = 0;
        let count_ptr: *mut u32 = &mut count;
        let mut events_buffer: FFI_DL_EventList = std::ptr::null();
        let events_ptr: *mut FFI_DL_EventList = &mut events_buffer;
        let res = DL_GetEvents(count_ptr, events_ptr);
        if res != 0 {
            panic!("DL_GetEvents failed! res={}", res);
        }

        // FIXME: blech an allocation on every get_events call? what can I do to get rid of this?
        let mut safe_events = Vec::new();
        for event_ptr in std::slice::from_raw_parts(events_buffer, count as usize) {
            // Our lib should never return a nullptr so the unwrap matches the API guarantee
            let event = event_ptr.as_ref().unwrap();
            let safe_event = match event.event_type {
                FFI_DL_EventType::Created => {
                    // FIXME: blech an alloc just to provide a rust-safe cstring? maybe I should
                    // just be returning rust's CString type? not sure...
                    let creationCStr =
                        std::mem::transmute::<&FFI_DL_Event, &FFI_DL_CreatedEvent>(event)
                            .creationString;
                    // FIXME: this is a mess. two allocations. unwrap assumptions. cleanup pls
                    let creationString =
                        CString::new(CStr::from_ptr(creationCStr).to_string_lossy().into_owned())
                            .unwrap();
                    DlEvent::CreatedEvent { creationString }
                }
                FFI_DL_EventType::Destroyed => {
                    let destroyedByte =
                        std::mem::transmute::<&FFI_DL_Event, &FFI_DL_DestroyedEvent>(event)
                            .destroyedByte;
                    DlEvent::DestroyedEvent { destroyedByte }
                }
                FFI_DL_EventType::Changed => {
                    let changedState =
                        std::mem::transmute::<&FFI_DL_Event, &FFI_DL_ChangedEvent>(event)
                            .changedState;
                    DlEvent::ChangedEvent { changedState }
                }
            };

            safe_events.push(safe_event)
        }

        DlEventList {
            events: safe_events,
            return_ptr: events_buffer,
        }
    }
}

pub fn return_events(event_list: DlEventList) {
    unsafe {
        let res = DL_ReturnEvents(event_list.events.len() as u32, event_list.return_ptr);
        if res != 0 {
            panic!("DL_ReturnEvents failed! res={}", res);
        }
    }
}

pub fn handle_created_event(creationString: &CStr) {
    unsafe {
        let res = DL_HandleCreatedEvent(creationString.as_ptr());
        if res != 0 {
            panic!("DL_HandleCreatedEvent failed! res={}", res);
        }
    }
}

pub fn handle_destroyed_event(destroyedByte: u8) {
    unsafe {
        let res = DL_HandleDestroyedEvent(destroyedByte);
        if res != 0 {
            panic!("DL_HandleDestroyedEvent failed! res={}", res);
        }
    }
}

pub fn handle_changed_event(changedState: ChangedState) {
    unsafe {
        let res = DL_HandleChangedEvent(changedState);
        if res != 0 {
            panic!("DL_HandleChangedEvent failed! res={}", res);
        }
    }
}

#[cfg(test)]
mod failure_tests {
    use super::*;

    #[test]
    #[should_panic]
    fn cant_return_before_get() {
        let events = Vec::new();
        let return_ptr = std::ptr::null();
        let event_list = DlEventList { events, return_ptr };
        return_events(event_list);
    }

    #[test]
    #[should_panic]
    fn cant_call_return_multiple_times_without_get() {
        let event_list = get_events();

        let events = Vec::new();
        let return_ptr = event_list.return_ptr;
        let event_list_2 = DlEventList { events, return_ptr };

        return_events(event_list);
        return_events(event_list_2);
    }

    #[test]
    #[should_panic]
    fn must_only_return_given_1() {
        let event_list = get_events();

        let events = Vec::new();
        let return_ptr = event_list.return_ptr;
        let event_list_2 = DlEventList { events, return_ptr };

        return_events(event_list_2);
    }

    #[test]
    #[should_panic]
    fn must_only_return_given_2() {
        let event_list = get_events();

        let mut events = Vec::new();
        events.push(DlEvent::DestroyedEvent { destroyedByte: 0 });
        let return_ptr = event_list.return_ptr;
        let event_list_2 = DlEventList { events, return_ptr };

        return_events(event_list_2);
    }

    #[test]
    #[should_panic]
    fn must_return_expected_creation_string() {
        let event_list = get_events();
        for event in &event_list.events {
            if let DlEvent::CreatedEvent { creationString } = event {
                let mut corruptCreationString = CString::new("GARBAGE").unwrap();
                handle_created_event(&corruptCreationString);
            }
        }
        return_events(event_list);
    }

    #[test]
    #[should_panic]
    fn must_return_expected_destroyed_byte() {
        let event_list = get_events();
        for event in &event_list.events {
            if let DlEvent::DestroyedEvent { destroyedByte } = event {
                handle_destroyed_event(destroyedByte + 1);
            }
        }
    }

    #[test]
    #[should_panic]
    fn must_return_expected_changed_state() {
        let event_list = get_events();
        for event in &event_list.events {
            if let DlEvent::ChangedEvent { changedState } = event {
                let wrongState = match changedState {
                    ChangedState::A => ChangedState::B,
                    ChangedState::B => ChangedState::A,
                };

                handle_changed_event(wrongState);
            }
        }
    }
}
