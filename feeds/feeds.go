package feeds

type FeedItemAccessor interface {
    Title() string
    Link() string
    DataAsHtml() string
}

type FeedReader interface {
    SleepUntilScheduled()
    ReadFeed(maxFeedItems uint8) []FeedItemAccessor
}
