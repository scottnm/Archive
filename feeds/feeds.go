package feeds

type FeedItemAccessor interface {
    Title() string
    Link() string
    DataAsHTML() string
}

type FeedReader interface {
    SleepUntilScheduled()
    ReadFeed(maxFeedItems uint8) []*FeedItemAccessor
}
