# TimeJournal
A program that pesters me with a text editor periodically to track how I'm spending my time throughout the day. This is inspired by Mike Acton's 12 tips for making better use of your time. https://sites.google.com/site/macton/home/gamedev12tipsformakingthebestuseofyourtime

## Goals

- allow configuring "work hours"
- Once an hour, prompt the user to record what they'd done in the last hour
- if prompting at the end or after work hours, also prompt user to be done with the day.
- once user is done at the end of the day, email user with log for the day

## Ideas

- store config bits in a file (think .gitconfig but a json file)
- use threadpool to put process to sleep on timers
- for testing configure timer to open on shorter timespan, automate a write, verify that N files are written

## Breakdown of work

- [] compile hello world
- [] get program to run without window
- [] use threadpools to run some procedure once every period
- [] figure out how to open text editor programatically with a name that identifies what the log represents (e.g. the 4pm log or the 4th log of the day or whatever)
- [] write tests as part of major pieces
- [] use threadpools to run some text editor procedure once every period
- [] read period/work hours from config file
- [] allow extending work hours via some prompt
- [] collect logs/journal entries into a report
- [] figure out how to automatically email report (or some other shortcut that sends a report to my phone)
