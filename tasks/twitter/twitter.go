package twitter

import (
    "bytes"
    "encoding/json"
    "fmt"
    "github.com/kurrik/oauth1a"
    "github.com/kurrik/twittergo"
    "io/ioutil"
    "net/http"
    "net/url"
    "strconv"
)

type TwitterFeeder struct {
    ScreenName string
    client *twittergo.Client
}

func GetStringJsonFromFile(fileName string) map[string]string {
    jsonBytes, err := ioutil.ReadFile(fileName)
    if err != nil {
        panic(fmt.Sprintf("Couldn't read file [%s] :: %s", fileName, err.Error()))
    }
    var jsonObj map[string]string
    err = json.Unmarshal(jsonBytes, &jsonObj)
    if err != nil {
        panic(fmt.Sprintf("Couldnt interpret bytes as json [%s] :: %s", jsonBytes, err.Error()))
    }
    return jsonObj
}

func GetClient(consumerKey, consumerSecret, accessToken, accessTokenSecret string) *twittergo.Client {
    clientConfig := &oauth1a.ClientConfig{ConsumerKey: consumerKey, ConsumerSecret: consumerSecret}
    user := oauth1a.NewAuthorizedConfig(accessToken, accessTokenSecret)
    client := twittergo.NewClient(clientConfig, user)
    return client
}

func NewTwitterFeeder(feedCredentials, feedConfig string) *TwitterFeeder {
    tf := new(TwitterFeeder)

    configAsJson := GetStringJsonFromFile(feedConfig)
    tf.ScreenName = configAsJson["ScreenName"]

    credentials := GetStringJsonFromFile(feedCredentials)
    twitterClient := GetClient(credentials["ConsumerKey"], credentials["ConsumerSecret"], credentials["AccessToken"], credentials["AccessTokenSecret"])
    tf.client = twitterClient

    return tf
}

func (tf *TwitterFeeder) GetFeed (count uint8) []string {
    query := url.Values{}
    query.Set("count", strconv.FormatUint(uint64(count), 10))
    query.Set("screen_name", tf.ScreenName)

    endpoint := fmt.Sprintf("/1.1/statuses/user_timeline.json?%v", query.Encode())
    req, err := http.NewRequest("GET", endpoint, nil)
    if err != nil {
        panic(fmt.Sprintf("Could not parse request: %v\n", err))
    }
    resp, err := tf.client.SendRequest(req)
    if err != nil {
        panic(fmt.Sprintf("Could not send request: %v\n", err))
    }
    timeline := &twittergo.Timeline{}
    err = resp.Parse(timeline)
    if err != nil {
        panic(fmt.Sprintf("Could not parse response: %v\n", err))
    }

    numTweetsRetrieved := len(*timeline)
    tweets := make([]string, numTweetsRetrieved)
    for i, tweet := range *timeline {
        tweetJsonBytes, err := json.Marshal(tweet)
        if err != nil {
            panic(fmt.Sprintf("Could not encode tweet %v\n", err))
        }
        var indentedJsonBytes bytes.Buffer
        json.Indent(&indentedJsonBytes, tweetJsonBytes, "", "    ")
        tweets[i] = indentedJsonBytes.String()
    }

    return tweets
}
