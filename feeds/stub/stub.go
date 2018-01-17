package stub

import (
    "rsserver/feeds"
)

type StubFeedReader struct {
    a uint8
    b uint8
}

func NewStubFeedReader(a, b uint8) *StubFeedReader {
    sf := new(StubFeedReader)
    sf.a = a
    sf.b = b
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
