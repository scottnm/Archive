package feeds

type FeedItem interface {
    Title() string
    Link() string
    DataAsHtml() string
}

type FeedReader interface {
    SleepUntilScheduled()
    ReadFeed() []FeedItem
}
