mod demo_lib;
use demo_lib::DlEvent;

fn main() {
    let event_list = demo_lib::get_events();
    println!(
        "count={}, event_list={:?}",
        event_list.events.len(),
        event_list.events.as_ptr()
    );
    for event in &event_list.events {
        println!("Processing event of type {:?}", event);
        match event {
            DlEvent::CreatedEvent { creationString } => {
                demo_lib::handle_created_event(creationString)
            }
            DlEvent::DestroyedEvent { destroyedByte } => {
                demo_lib::handle_destroyed_event(*destroyedByte)
            }
            DlEvent::ChangedEvent { changedState } => demo_lib::handle_changed_event(*changedState),
        }
    }
    demo_lib::return_events(event_list);
    println!("Done!");
}
