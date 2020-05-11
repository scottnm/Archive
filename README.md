# TimeJournal
A program that pesters me with a text editor periodically to track how I'm spending my time throughout the day. This is inspired by Mike Acton's 12 tips for making better use of your time. https://sites.google.com/site/macton/home/gamedev12tipsformakingthebestuseofyourtime

## Goals

- allow configuring "work hours"
- allow configuring of report period
- Once per period, prompt the user to record what they'd done in the last hour
- if prompting at the end or after work hours, also prompt user to be done with the day.
- once user is done at the end of the day, email user with log for the day
- email is formatted in an easily readable way

## TODO

| Done? | ID | Description |
|-------|----|-------------|
| :white_check_mark: | 01 | compile hello world |
| :white_check_mark: | 02 | get program to run without window |
| :white_check_mark: | 03 | use threadpools to run some procedure once every period |
|                    | 04 | figure out how to open text editor programatically with a name that identifies what the log represents (e.g. the 4pm log or the 4th log of the day or whatever) |
|                    | 05 | write tests as part of major pieces |
|                    | 06 | use threadpools to run some text editor procedure once every period |
| :white_check_mark: | 07 | read period/work hours from config file |
|                    | 08 | allow extending work hours via some prompt |
|                    | 09 | collect logs/journal entries into a report |
|                    | 10 | figure out how to automatically email report (or some other shortcut that sends a report to my phone) |
|                    | 11 | ensure program is booted on startup |
|                    | 12 | ensure program resets itself on next day |
|                    | 13 | enable way of telling program you'll be gone for some period of time (to ignore prompts during that away time) |
|                    | 14 | add some toolbar icon to ease interacting with it in the background (button for 'set away', 'set here', 'exit') |
|                    | 15 | support guided installation |
|                    | 16 | provide setup mechanism to create local appdata folder |
|                    | 17 | provide setup mechanism to generate config file |
|                    | 18 | provide some toolbar functionality that allows you to easily edit the config file |

### Edge cases to consider

- What happens if you don't use it at all for the day? Does it send any email?

## Implementation Ideas

- store config bits in a file (think .gitconfig but a json file)
- use threadpool to put process to sleep on timers
- for testing configure timer to open on shorter timespan, automate a write, verify that N files are written
- use json file for ease of reading/writing logs

# What permissions does it need?

* permissions to read/write files on disk
* permissions to email
* permissions to run program headless/in-background
