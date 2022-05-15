package main

import (
    "encoding/gob"
    "fmt"
    "os"
    //"rsserver/tasks/twitter"
    "rsserver/feeds"
    "rsserver/feeds/stub"
    "sync"
)

func RunFeedReader(readCount uint8, wg *sync.WaitGroup, feedItemSink chan []feeds.FeedItem, feedReader feeds.FeedReader) {
    wg.Add(1)
    go func() {
        for i := uint8(0); i < readCount; i++ {
            feedReader.SleepUntilScheduled()
            newFeedItems := feedReader.ReadFeed()
            feedItemSink <- newFeedItems
        }
        wg.Done()
    }()
}

func LoadPreviousFeed() []feeds.FeedItem {
    var feedItems []feeds.FeedItem

    file, err := os.Open("saved.feed")
    if err != nil {
        fmt.Println("Failed to find previous feed!")
        return make([]feeds.FeedItem, 0)
    }
    defer file.Close()

    decoder := gob.NewDecoder(file)
    if e := decoder.Decode(&feedItems); e != nil {
        fmt.Println("Failed to read previous feed!")
    }

    return feedItems
}

func SaveFeed(feed []feeds.FeedItem) {
    file, err := os.OpenFile("saved.feed", os.O_WRONLY | os.O_CREATE, 0644)
    if err != nil {
        panic("Failed to open feed save file!")
    }
    defer file.Close()

    encoder := gob.NewEncoder(file)
    if e := encoder.Encode(feed); e != nil {
        fmt.Println("%v", e);
        panic("Failed to save feed!")
    }
}

func PrintFeed(previousFeedItems []feeds.FeedItem){
    for _, item := range previousFeedItems {
        fmt.Printf("Title: %v\tLink: %v\tDataAsHtml: %v\n", item.Title(), item.Link(), item.DataAsHtml())
    }
}

func main() {
    //feed := twitter.NewTwitterFeeder("tasks/twitter/credentials.secret.json", "tasks/twitter/config.json")
    gob.Register(stub.StubFeedItem{})

    // Load previous items
    allFeedItems := LoadPreviousFeed()
    fmt.Println("Printing previous feed items...")
    PrintFeed(allFeedItems)

    // Read new items from the feed
    fmt.Println("Loading new feed items...")
    feedItemSink := make(chan []feeds.FeedItem)
    waitGroup := new(sync.WaitGroup)
    for i := uint8(1); i <= 3; i++ {
        RunFeedReader(i, waitGroup, feedItemSink, stub.NewStubFeedReader(i, i * 3))
    }

    // Close the feed item sink channel once all of the feed readers finish reading.
    go func() {
        waitGroup.Wait()
        close(feedItemSink)
    }()

    // Print the result from the feed channel
    for feedItemResponse := range feedItemSink {
        PrintFeed(feedItemResponse)
        allFeedItems = append(allFeedItems, feedItemResponse...)
    }

    SaveFeed(allFeedItems)

    fmt.Println("Done.")
}
