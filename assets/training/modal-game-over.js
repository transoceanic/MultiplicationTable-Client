var flow = require('Flow');

cc.Class({
    extends: cc.Component,

    properties: {
        score: cc.Label,
        isIncrease: cc.Label
    },

    // use this for initialization
    onLoad: function () {
        
    },
    
    setScore: function(score) {
        this.score.string = score;
        this.isIncrease.node.active = flow.setMyScore(score);
    }

    // called every frame, uncomment this function to activate update callback
    // update: function (dt) {

    // },
});
