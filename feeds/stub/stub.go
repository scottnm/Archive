package stub

import (
    "fmt"
    "math/rand"
    "rsserver/feeds"
    "time"
)

type StubFeedItem struct {
    title string
    link string
    dataAsHtml string
}

func (sfi StubFeedItem) Title() string {
    return sfi.title
}

func (sfi StubFeedItem) Link() string {
    return sfi.link
}

func (sfi StubFeedItem) DataAsHtml() string {
    return sfi.dataAsHtml
}

type StubFeedReader struct {
    id uint8
    randVal uint8
    sleepTime uint8
}

func NewStubFeedReader(id, sleepTime uint8) *StubFeedReader {
    sf := new(StubFeedReader)
    sf.id = id

    rand.Seed(time.Now().UTC().UnixNano())
    sf.randVal = uint8(rand.Intn(256))

    sf.sleepTime = sleepTime
    return sf
}

func (sf *StubFeedReader) SleepUntilScheduled() {
    getTimeAfterOp := time.After(time.Second * time.Duration(sf.sleepTime))
    <-getTimeAfterOp
}

func (sf *StubFeedReader) ReadFeed() []feeds.FeedItem {
    items := make([]feeds.FeedItem, 1)
    for i := uint8(0); i < 1; i++ {
        newItem := new(StubFeedItem)
        newItem.title = fmt.Sprintf("T%d-%d", sf.id, sf.randVal)
        newItem.link = fmt.Sprintf("http://link.com/%d", sf.id)
        newItem.dataAsHtml = fmt.Sprintf("<div>%d</div>", sf.id)
        items[i] = newItem
    }
    return items
}
