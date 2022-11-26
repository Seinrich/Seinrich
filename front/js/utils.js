"use strict";
//步数
let count = 0;
//阴影坐标
let shadex = 0, shadey = 0;
//map
let abxTOxMap = new Map();
let abyTOyMap = new Map();
//棋子自适应大小
let qizis = [];
//吃子
let ns = [], tns = [];
//棋盘
let chess = Array.from(Array(19), () => new Array(19));

//初始化棋盘
function initChess() {
    for (let i = 0; i < chess.length; i++) {
        for (let j = 0; j < chess[i].length; j++) {
            chess[i][j] = 0;
        }
    }
}

//音效
const domusic = new Audio('resource/sound/do.wav');
const eatmusic = new Audio('resource/sound/eat.wav');
const errormusic = new Audio('resource/sound/error.wav');

//坐标
class XY {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

//清空棋子
function clearQizi() {
    while (count > 0) {
        count--;
        //如果已被吃则跳过
        if (document.getElementById("qizi" + count) !== null) {
            document.getElementById("qizi" + count).remove();
        }
        qizis.pop();
    }
    initChess();
}

//判断吃子
function eat(x, y) {
    ns = [];
    let opposite = (count + 1) % 2;
    if (x + 1 <= 18 && chess[x + 1][y] % 2 === opposite && chess[x + 1][y] > 0) {
        if (qi(x + 1, y, opposite)) {
            ns.push(tns);
        }
        tns = [];
    }
    if (x - 1 >= 0 && chess[x - 1][y] % 2 === opposite && chess[x - 1][y] > 0) {
        if (qi(x - 1, y, opposite)) {
            ns.push(tns);
        }
        tns = [];
    }
    if (y + 1 <= 18 && chess[x][y + 1] % 2 === opposite && chess[x][y + 1] > 0) {
        if (qi(x, y + 1, opposite)) {
            ns.push(tns);
        }
        tns = [];
    }
    if (y - 1 >= 0 && chess[x][y - 1] % 2 === opposite && chess[x][y - 1] > 0) {
        if (qi(x, y - 1, opposite)) {
            ns.push(tns);
        }
        tns = [];
    }

    //去重
    ns[0] = Array.from(new Set(ns[0]));
}

//判断气
function qi(x, y, opposite) {
    let temp = chess[x][y];
    tns.push(temp - 1);
    chess[x][y] = -1;
    if ((x - 1 >= 0 && chess[x - 1][y] === 0) || (x + 1 <= 18 && chess[x + 1][y] === 0)
        || (y - 1 >= 0 && chess[x][y - 1] === 0) || (y + 1 <= 18 && chess[x][y + 1] === 0)) {
        chess[x][y] = temp;
        return false;
    }
    if (x - 1 >= 0 && chess[x - 1][y] > 0 && chess[x - 1][y] % 2 === opposite) {
        if (!qi(x - 1, y, opposite)) {
            chess[x][y] = temp;
            return false;
        }
    }
    if (x + 1 <= 18 && chess[x + 1][y] > 0 && chess[x + 1][y] % 2 === opposite) {
        if (!qi(x + 1, y, opposite)) {
            chess[x][y] = temp;
            return false;
        }
    }
    if (y + 1 <= 18 && chess[x][y + 1] > 0 && chess[x][y + 1] % 2 === opposite) {
        if (!qi(x, y + 1, opposite)) {
            chess[x][y] = temp;
            return false;
        }
    }
    if (y - 1 >= 0 && chess[x][y - 1] > 0 && chess[x][y - 1] % 2 === opposite) {
        if (!qi(x, y - 1, opposite)) {
            chess[x][y] = temp;
            return false;
        }
    }
    chess[x][y] = temp;
    return true;
}

//在棋盘上落子
function luoziInImg(x, y) {
    let qipan = document.getElementById("qipan");
    let image = document.getElementById("image");
    let qizi = Qizi();
    qizi.style.display = 'block';
    qizi.style.zIndex = '25';
    qizi.style.position = 'absolute';
    image.insertBefore(qizi, image.children[0]);
    jQizi().offset({top: abyToY(y) + qipan.offsetTop - 6, left: abxToX(x) + qipan.offsetLeft - 9});

    //消除阴影
    shadey = undefined;
    shadex = undefined;
    Shade().style.display = 'none';

    //棋子数组
    qizis.push(new XY(x, y));

    //棋盘
    chess[x][y] = qizis.length;

    //棋子数+1
    count++;
}

//睡眠
const sleep = (delay) => new Promise((resolve) => setTimeout(resolve, delay));

//返回阴影元素
function Shade() {
    if (count % 2 === 0) {
        return document.getElementById('blackshade');
    } else {
        return document.getElementById('whiteshade');
    }
}

//返回阴影jquery对象
function jShade() {
    if (count % 2 === 0) {
        return $('#blackshade');
    } else {
        return $('#whiteshade');
    }
}

//监听阴影移动
document.addEventListener("mousemove", function (e) {
    if (e.target.id !== null) {
        //在棋盘上
        if (e.target.id === "qipan"
            && e.offsetX < 900 && e.offsetX > 65 && e.offsetY < 820 && e.offsetY > 48) {
            //设置阴影属性
            let qipan = document.getElementById("qipan");
            let shade = Shade();
            shade.style.display = 'block';
            shade.style.zIndex = '20';
            shade.style.position = 'absolute';
            //记录阴影坐标
            shadey = correctY(e.offsetY);
            shadex = correctX(e.offsetX);
            jShade().offset(
                {top: shadey + qipan.offsetTop, left: shadex + qipan.offsetLeft});
        }
        //鼠标不在棋盘上是阴影消失
        else {
            if (e.target.id !== 'blackshade' && e.target.id !== 'whiteshade') {
                if (e.target.id !== "qipan") {
                    shadey = undefined;
                    shadex = undefined;
                    Shade().style.display = 'none';
                }
            }
        }
    }
});

//返回棋子元素
function Qizi() {
    if (count % 2 === 0) {
        let black = document.createElement('img');
        black.src = 'resource/image/黑棋子.png';
        black.id = 'qizi' + count;
        return black;
    } else {
        let white = document.createElement('img');
        white.src = 'resource/image/白棋子.png';
        white.id = 'qizi' + count;
        return white;
    }
}

//返回棋子jquery对象
function jQizi() {
    return $('#qizi' + count);
}

//返回修正后棋盘x坐标
function correctX(x) {
    if (x > 65 && x <= 97) return 72;
    else if (x > 97 && x <= 143) return 118;
    else if (x > 143 && x <= 189) return 164;
    else if (x > 189 && x <= 233) return 208;
    else if (x > 233 && x <= 279) return 253;
    else if (x > 279 && x <= 324) return 298;
    else if (x > 324 && x <= 370) return 343;
    else if (x > 370 && x <= 414) return 387;
    else if (x > 414 && x <= 460) return 432;
    else if (x > 460 && x <= 506) return 476;
    else if (x > 506 && x <= 547) return 520;
    else if (x > 547 && x <= 593) return 564;
    else if (x > 593 && x <= 638) return 611;
    else if (x > 638 && x <= 680) return 656;
    else if (x > 680 && x <= 721) return 700;
    else if (x > 721 && x <= 770) return 743;
    else if (x > 770 && x <= 815) return 788;
    else if (x > 815 && x <= 861) return 832;
    else if (x > 861 && x <= 900) return 879;
}

//返回修正后棋盘y坐标
function correctY(y) {
    if (y > 48 && y <= 77) return 57;
    else if (y > 77 && y <= 122) return 98;
    else if (y > 122 && y <= 166) return 139;
    else if (y > 166 && y <= 190) return 179;
    else if (y > 190 && y <= 248) return 218;
    else if (y > 248 && y <= 283) return 263;
    else if (y > 283 && y <= 322) return 302;
    else if (y > 322 && y <= 362) return 342;
    else if (y > 362 && y <= 403) return 383;
    else if (y > 403 && y <= 447) return 425;
    else if (y > 447 && y <= 489) return 468;
    else if (y > 489 && y <= 536) return 510;
    else if (y > 536 && y <= 578) return 550;
    else if (y > 578 && y <= 620) return 592;
    else if (y > 620 && y <= 660) return 634;
    else if (y > 660 && y <= 700) return 677;
    else if (y > 700 && y <= 745) return 719;
    else if (y > 745 && y <= 785) return 761;
    else if (y > 785 && y <= 820) return 803;
}

//返回修正后棋盘x坐标
function abx(x) {
    if (x > 65 && x <= 97) return 0;
    else if (x > 97 && x <= 143) return 1;
    else if (x > 143 && x <= 189) return 2;
    else if (x > 189 && x <= 233) return 3;
    else if (x > 233 && x <= 279) return 4;
    else if (x > 279 && x <= 324) return 5;
    else if (x > 324 && x <= 370) return 6;
    else if (x > 370 && x <= 414) return 7;
    else if (x > 414 && x <= 460) return 8;
    else if (x > 460 && x <= 506) return 9;
    else if (x > 506 && x <= 547) return 10;
    else if (x > 547 && x <= 593) return 11;
    else if (x > 593 && x <= 638) return 12;
    else if (x > 638 && x <= 680) return 13;
    else if (x > 680 && x <= 721) return 14;
    else if (x > 721 && x <= 770) return 15;
    else if (x > 770 && x <= 815) return 16;
    else if (x > 815 && x <= 861) return 17;
    else if (x > 861 && x <= 900) return 18;
    else {
        return abx(shadex);
    }
}

//返回修正后棋盘y坐标
function aby(y) {
    if (y > 48 && y <= 77) return 0;
    else if (y > 77 && y <= 122) return 1;
    else if (y > 122 && y <= 166) return 2;
    else if (y > 166 && y <= 190) return 3;
    else if (y > 190 && y <= 248) return 4;
    else if (y > 248 && y <= 283) return 5;
    else if (y > 283 && y <= 322) return 6;
    else if (y > 322 && y <= 362) return 7;
    else if (y > 362 && y <= 403) return 8;
    else if (y > 403 && y <= 447) return 9;
    else if (y > 447 && y <= 489) return 10;
    else if (y > 489 && y <= 536) return 11;
    else if (y > 536 && y <= 578) return 12;
    else if (y > 578 && y <= 620) return 13;
    else if (y > 620 && y <= 660) return 14;
    else if (y > 660 && y <= 700) return 15;
    else if (y > 700 && y <= 745) return 16;
    else if (y > 745 && y <= 785) return 17;
    else if (y > 785 && y <= 820) return 18;
    else {
        return aby(shadey);
    }
}

function abxToX(x) {
    return abxTOxMap.get(x);
}

function abyToY(y) {
    return abyTOyMap.get(y);
}

//map
(function () {
    abxTOxMap.set(0, 72);
    abxTOxMap.set(1, 118);
    abxTOxMap.set(2, 164);
    abxTOxMap.set(3, 208);
    abxTOxMap.set(4, 253);
    abxTOxMap.set(5, 298);
    abxTOxMap.set(6, 343);
    abxTOxMap.set(7, 387);
    abxTOxMap.set(8, 432);
    abxTOxMap.set(9, 476);
    abxTOxMap.set(10, 520);
    abxTOxMap.set(11, 564);
    abxTOxMap.set(12, 611);
    abxTOxMap.set(13, 656);
    abxTOxMap.set(14, 700);
    abxTOxMap.set(15, 743);
    abxTOxMap.set(16, 788);
    abxTOxMap.set(17, 832);
    abxTOxMap.set(18, 879);

    abyTOyMap.set(0, 57);
    abyTOyMap.set(1, 98);
    abyTOyMap.set(2, 139);
    abyTOyMap.set(3, 179);
    abyTOyMap.set(4, 218);
    abyTOyMap.set(5, 263);
    abyTOyMap.set(6, 302);
    abyTOyMap.set(7, 342);
    abyTOyMap.set(8, 383);
    abyTOyMap.set(9, 425);
    abyTOyMap.set(10, 468);
    abyTOyMap.set(11, 510);
    abyTOyMap.set(12, 550);
    abyTOyMap.set(13, 592);
    abyTOyMap.set(14, 634);
    abyTOyMap.set(15, 677);
    abyTOyMap.set(16, 719);
    abyTOyMap.set(17, 761);
    abyTOyMap.set(18, 803);
})();