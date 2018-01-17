package stub

import (
    "rsserver/feeds"
)

type StubFeedReader struct {
}

func NewStubFeedReader() *StubFeedReader {
    sf := new(StubFeedReader)
    return sf
}

func (sf *StubFeedReader) SleepUntilScheduled() {
}

func (sf *StubFeedReader) ReadFeed(maxFeedItems uint8) []*feeds.FeedItemAccessor {
    if maxFeedItems == 0 {
        return make([]*feeds.FeedItemAccessor, 0)
    }
    return make([]*feeds.FeedItemAccessor, 0)
}
