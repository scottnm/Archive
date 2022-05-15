var app = angular.module("PollApp", []);
app.controller("PreviewController", function(){
    this.polls = polls;
    /**
     * function that determines an answer choice's vote percentage
     * @param poll - The poll within the list of polls to calculate a percentage
     *               for
     * @param choice - The answer choice to calculate a percentage for the
     *                 specific poll
     */
    this.votePercentage = function(poll, choice){
        return (choice["votes"] / poll["votes"] * 100).toFixed(0);
    }
});