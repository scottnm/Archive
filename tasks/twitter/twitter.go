package twitter

type TwitterFeeder struct {
    ScreenName string
}

func NewTwitterFeeder (screenName string) *TwitterFeeder {
    tf := new(TwitterFeeder)
    tf.ScreenName = screenName
    return tf
}

func (tf *TwitterFeeder) GetFeed () []string {
    return []string{tf.ScreenName}
}
