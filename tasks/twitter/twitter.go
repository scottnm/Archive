package twitter

import (
    "encoding/json"
    "fmt"
    "github.com/kurrik/oauth1a"
    "github.com/kurrik/twittergo"
    "io/ioutil"
)

type TwitterFeeder struct {
    ScreenName string
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

    fmt.Printf("Credentials...\n")
    fmt.Printf("\tCredentialsObj: %s\n", credentials)
    fmt.Printf("\tTwitterClient: %s\n", twitterClient)
    fmt.Printf("...End Credentials\n")
    return tf
}

func (tf *TwitterFeeder) GetFeed () []string {
    return []string{tf.ScreenName}
}
