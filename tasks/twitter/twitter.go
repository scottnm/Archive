package twitter

import (
    "encoding/json"
    "fmt"
    "github.com/kurrik/oauth1a"
    "github.com/kurrik/twittergo"
    "io/ioutil"
    "net/http"
    "strconv"
)

type TwitterFeeder struct {
    ScreenName string
    Id uint64
    Name string
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

    req, err := http.NewRequest("GET", "/1.1/account/verify_credentials.json", nil)
    if err != nil {
        panic(fmt.Sprintf("Could not parse request: %v\n", err))
    }

    resp, err := twitterClient.SendRequest(req)
    if err != nil {
        panic(fmt.Sprintf("Could not send request: %v\n", err))
    }

    user := &twittergo.User{}
    err = resp.Parse(user)
    if err != nil {
        panic(fmt.Sprintf("Could not parse response: %v\n", err))
    }

    fmt.Printf("ID:\t%v\n", user.Id())
    fmt.Printf("Name:\t%v\n", user.Name())

    tf.Id = user.Id()
    tf.Name = user.Name()

    return tf
}

func (tf *TwitterFeeder) GetFeed () []string {
    return []string{tf.ScreenName, strconv.FormatUint(tf.Id, 10), tf.Name}
}
