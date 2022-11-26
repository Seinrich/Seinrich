"use strict";

//提示数
let tipcount = 0;
//棋子自适应大小
let tipqizis = [];
//落子时坐标
let lx = -1, ly = -1;
//判断棋盘上是否有提示
let hasTip = false;

//布局属性
class LayoutAttribute {
    constructor(name, location, index) {
        this.name = name;
        this.location = location;
        this.index = index;
    }
}

let layout_attribute = new LayoutAttribute('null', 'null', -1);

//页面加载时
window.onload = function () {
    initChess();
    flush();
}

//命令
class Command {
    constructor(type, x, y, degree) {
        this.type = type;//类型
        this.x = x;//x坐标
        this.y = y;//y坐标
        this.degree = degree;//难度
    }
}

//数据
class Data {
    constructor(command, qizis, layout_attribute) {
        this.command = command;//命令
        this.qizis = qizis;//棋谱
        this.layout_attribute = layout_attribute;//布局属性
    }
}

//提示
function tip() {
    if (hasTip) {
        hasTip = false;
        clearTipQizi();
    } else {
        hasTip = true;
        let degree = $("input[name='model']:checked").val();
        sendData(new Data(new Command("tip", -1, -1, degree), qizis, layout_attribute));
    }
}

//刷新
function flush() {
    let degree = $("input[name='model']:checked").val();
    sendData(new Data(new Command('flush', -1, -1, degree), qizis, layout_attribute));
}

//落子
function luozi(x, y) {
    lx = x;
    ly = y;
    let degree = $("input[name='model']:checked").val();
    sendData(new Data(new Command('luozi', x, y, degree), qizis, layout_attribute));
}

//发送请求
function sendData(data) {
    //从服务器接收初始化数据
    $.ajax({
        type: "GET",
        url: "http://localhost:8088/",
        data: JSON.stringify(data),
        dataType: "json",
        async: false,
        success: function (data) {
            console.log("success");
            dispose(data);
        },
        // error: function (status) {
        //     console.log(status);
        // }
        error: function (jqXHR, textStatus, error) {
            console.log(jqXHR, textStatus, error);
        }
    });
}

//清空提示
function clearTipQizi() {
    while (tipcount > 0) {
        tipcount--;
        document.getElementById("tip" + tipcount).remove();
        tipqizis.pop();
    }
}

//处理数据
async function dispose(data) {
    //失败
    if (data.type === "fail") {
        console.log("错误");
        await errormusic.play();
    }
    //结束
    else if (data.type === 'finish') {
        executeLuozi(new XY(lx, ly));
        console.log("落子成功");
        await sleep(500);
        //如果ai走棋后定式结束执行
        if ('xy' in data) {
            executeLuozi(data.xy);
        }
        await sleep(400);
        alert("定式完成");
        flush();
    }
    //落子
    else if (data.type === 'luozi') {
        executeLuozi(new XY(lx, ly));
        console.log("落子成功");
        await sleep(500);
        executeLuozi(data.xy);
    }
    //提示
    else if (data.type === 'tip') {
        console.log("提示成功");
        executeTip(data.xys);
    }
    //刷新
    else if (data.type === 'flush') {
        console.log("刷新成功");
        layout_attribute = data.layout_attribute;
        executeFlush(data.xys);
    }
    //其他
    else {
        console.log(data);
    }
}

//执行落子 传xy
//xy:x,y
function executeLuozi(xy) {
    //清空提示
    clearTipQizi();
    //在棋盘上落子和吃子
    //data为一步棋
    LuoAndEatInImg(xy, true);
}

//执行提示 xy数组
//xy: x,y
function executeTip(xys) {
    //获得父元素
    let qipan = document.getElementById('qipan');
    let image = document.getElementById('image');

    //data为坐标数组
    for (let xy of xys) {
        let tipqizi = tipQizi();
        tipqizi.style.display = 'block';
        tipqizi.style.zIndex = '25';
        tipqizi.style.position = 'absolute';
        image.insertBefore(tipqizi, image.children[0]);
        jTipqizi().offset({top: abyToY(xy.y) + qipan.offsetTop - 8, left: abxToX(xy.x) + qipan.offsetLeft - 11});

        tipqizis.push(new XY(xy.x, xy.y));
        tipcount++;
    }
}

//执行刷新 传xy数组
//xy:x,y
function executeFlush(xys) {
    //先清空棋子
    clearQizi();
    clearTipQizi();

    //data为棋谱
    for (let xy of xys) {
        //棋盘上落子并吃子
        LuoAndEatInImg(xy, false);
    }
}

//在棋盘上落子并吃子
function LuoAndEatInImg(xy, bool) {
    //落子
    luoziInImg(xy.x, xy.y);
    if (bool === true) {
        domusic.play();
    }

    //吃子
    eat();

    //吃子
    if (ns[0].length !== 0) {
        for (let n of ns[0]) {
            document.getElementById('qizi' + n).remove();
            //不传bu改为传xy后不改变棋谱
            //qizis.splice(n, 1);
        }
        if (bool === true) {
            eatmusic.play();
        }
    }
}

//自适应比例
window.onresize = function () {
    for (let i = 0; i < qizis.length; i++) {
        let xy = qizis.at(i);
        $('#qizi' + i).offset({top: abyToY(xy.y) + qipan.offsetTop - 6, left: abxToX(xy.x) + qipan.offsetLeft - 9})
    }
    for (let i = 0; i < tipqizis.length; i++) {
        let xy = tipqizis.at(i);
        $('#tip' + i).offset({top: abyToY(xy.y) + qipan.offsetTop - 6, left: abxToX(xy.x) + qipan.offsetLeft - 9})
    }
    jShade().offset({top: shadey + qipan.offsetTop, left: shadex + qipan.offsetLeft});
}

//返回提示棋子元素
function tipQizi() {
    if (count % 2 === 0) {
        let black = document.createElement('img');
        black.src = 'resource/image/黑棋子.png';
        black.id = 'tip' + tipcount;
        black.style.opacity = '0.5';
        return black;
    } else {
        let white = document.createElement('img');
        white.src = 'resource/image/白棋子.png';
        white.id = 'tip' + tipcount;
        white.style.opacity = '0.5';
        return white;
    }
}

//返回提示棋子jquery对象
function jTipqizi() {
    return $('#tip' + tipcount);
}

//监听落子
document.addEventListener('mouseup', function (e) {
    if (e.button === 0) {
        if (e.target.id !== null) {
            //判断在棋盘上
            if ((e.target.id === "qipan" && e.offsetX < 900 && e.offsetX > 50 && e.offsetY < 820 && e.offsetY > 40)
                || e.target.id === 'blackshade' || e.target.id === 'whiteshade') {
                if (Shade().style.display !== 'none') {
                    //判断是否越界
                    if (abx(e.offsetX) !== undefined && aby(e.offsetY) !== undefined
                        && chess[abx(e.offsetX)][aby(e.offsetY)] === 0) {
                        // luoziInImg(abx(e.offsetX), aby(e.offsetY));
                        luozi(abx(e.offsetX), aby(e.offsetY));
                    } else {
                        errormusic.play();
                    }
                } else {
                    errormusic.play();
                }
            }
            //玩家独有，在提示棋子上面
            else if (e.target.id === 'tip') {

            } else {
                let patt1 = /qizi/;
                let patt2 = /qipan/;
                if (patt1.test(e.target.id)) {
                    errormusic.play();
                } else if (patt2.test(e.target.id)) {
                    errormusic.play();
                }
            }
        }
    }
});