# rsserver
A server to coordinate the collection of information feeds, the curation of those collected feeds, and the delivery of
that curated feed to an accessible web page

# Design

### Definitions

**SourceFeed** - A 3rd party feed of information.

**InformationItem** - a single unit of information from a SourceFeed

**RssJob** - a specification of a SourceFeed, a schedule for when to retrieve that feed, and a set of rules for how to
filter that feed

### Functional requirements

* Manage a collection of RssJobs
* Run an RssJob on the it's defined schedule
* Collect new information from the SourceFeed of the RSS Job
* Filter information collected from the SourceFeed of an RSS Job
* Save filtered information into a database of some kind
    * Instapaper account
    * My own personally maintained DB
    * both (use personal db as archive and instapaper as transition tech until I get a web frontend to display my db)
* Present information from database in user friendly way
    * Just use Instapaper
    * Roll my own web frontend to the database
* From UI, be able to mark InformationItems as "read" to archive them and remove them front page of my feed
    * Instapaper "archive"
* Be able to view the "read"-archive in a separate view.
* Unread posts from a SourceFeed when a new set of InformationItems from that SourceFeed arrive are sent to an "unread"-archive
* Be able to view the "unread"-archive in a separate view.

# V0

* Scrape a twitter feed for new items
* Scrape once a week (during testing once per minute)
* Filter by a set of rules:
    * must contain a hyperlink (twitter native pictures and videos do not count)
* save items to instapaper
* save items to personal database (with tags for read vs unread and archive vs unarchived)
