# Behavior

* Once every 10 minutes a text editor appears
* In the text editor you enter what you've been working on for the last 10 minutes.
* Entries should be concise
* Save and close the editor and whatever you entered is appended to a log file
* If another 10 minutes elapses before the text editor is saved and closed, that 10 minute segment becomes invalid
* Program runs throughout the workday (9am -> 5pm)
* At the end of the day, program emails you the log file for you to review a summary of your day
* The log emailed to you should be formatted in a very easily readable way

# Edge cases

* What happens if you don't use it at all for the day? Does it send any email?

# Implementation questions

* What format will this log file be stored in? Text files are simple, but the security concerns me

# What may I need to write if pre-existing solutions don't exist?

* the component that sends an email

# Supporting tech

* a script to check if the program is running and if not, start running it if the time is appropriate.
* someway of interacting with the program to tell it you are going somewhere for a period of time and to delay until then
* a toolbar icon to kill it possibly


# What permissions does it need?

* permissions to read/write files on disk
* permissions to email
* permissions to run program headless/in-background
