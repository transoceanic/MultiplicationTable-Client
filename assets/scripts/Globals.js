window.G = {
    debug: false,

    properties: {
        domain: 'https://multiplication-table-server.herokuapp.com/',
        api: 'multiplication-table/',
        gameType: '10',
        rate: {
            iOS: 'itms-apps://itunes.apple.com/us/app/itunes-u/id1300602174?action=write-review',
            Android: 'market://details?id=com.abf.office.multiplicationtable10.en'
        },
        contactUs: 'game/{gameType}/en#contact2',
        privacyPolicy: 'policy/'
    },
    
    fadeInDuration: 0.2,
    fadeOutDuration: 0.2,
    answerTimeDuration: 5,
    rewardedAnswerTimeDuration: 5,
    gameplay: {
        version: 1,
        allowed: false,
        bestScore: null,
        awards: [],
        // {
        //     sprite: 0..3, // gold, silver, bronze, medal
        //     date: '30 jul 2017',
        //     top: 1, // 1,2,3,10,20,30..100
        //     period: 'week', // day, week, month, year, century
        // }
        tutorial: {
            level: true,
            okBtn: true
        }
    },
    settings: {
        name: null,
        music: true,
        sound: true
    },
    stat: {},
    levels: [
        {
            index: 0,
            number: 1,
            locked: false,
            position: cc.p(-140, -250),
            stars: {count: 0, isNew: false}
        },
        {
            index: 1,
            number: 2,
            locked: true,
            position: cc.p(0, -210),
            stars: {count: 0, isNew: false}
        },
        {
            index: 2,
            number: 3,
            locked: true,
            position: cc.p(140, -200),
            stars: {count: 0, isNew: false}
        },
        {
            index: 3,
            number: 4,
            locked: true,
            position: cc.p(-125, -400),
            stars: {count: 0, isNew: false}
        },
        {
            index: 4,
            number: 5,
            locked: true,
            position: cc.p(5, -360),
            stars: {count: 0, isNew: false}
        },
        {
            index: 5,
            number: 6,
            locked: true,
            position: cc.p(140, -350),
            stars: {count: 0, isNew: false}
        },
        {
            index: 6,
            number: 7,
            locked: true,
            position: cc.p(-110, -550),
            stars: {count: 0, isNew: false}
        },
        {
            index: 7,
            number: 8,
            locked: true,
            position: cc.p(12, -510),
            stars: {count: 0, isNew: false}
        },
        {
            index: 8,
            number: 9,
            locked: true,
            position: cc.p(140, -500),
            stars: {count: 0, isNew: false}
        },
        {
            index: 9,
            number: 10,
            locked: true,
            position: cc.p(20, -660),
            stars: {count: 0, isNew: false}
        }
    ],

    getName: function() {
        return this.settings.name || 'Guest';
    },
    
    save: function(key) {
        cc.sys.localStorage.setItem(key, JSON.stringify(this[key]));
    },

    restoreAll: function() {
        // cc.sys.localStorage.removeItem('levels');
        // cc.sys.localStorage.removeItem('settings');
        // cc.sys.localStorage.removeItem('gameplay');
        // cc.sys.localStorage.removeItem('stat');
        
        for (const key of ['levels', 'settings', 'gameplay', 'stat']) {
            let data = cc.sys.localStorage.getItem(key);
            if (data) {
                this[key] = JSON.parse( data );
                // console.log('restore '+key+' = '+JSON.stringify(this[key]));
            }
        }

        if (this.debug) {
            // this.gameplay.allowed = false;
            // this.settings.name = null;
            for (let i=0; i<10; i++) {
                this.levels[i].locked = false;
            }
        }
    }
};

G.restoreAll();
