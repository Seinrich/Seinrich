"use strict";

//页面加载时
window.onload = function () {
    initChess();
}

//数据
class Data {
    constructor(type, qizis) {
        this.type = type;
        this.qizis = qizis;
    }
}

//提交
function submit() {
    let type = $("input[name='model']:checked").val();
    sendData(new Data(type, qizis));
    clearQizi();
}

//发送请求
function sendData(data) {
    //从服务器接收初始化数据
    $.ajax({
        type: "get",
        url: "http://localhost:8088/",
        data: JSON.stringify(data),
        dataType: "json",
        async: false,
        success: function (data) {
            alert('提交成功');
            console.log(data);
            if(data === true){
                console.log('设置成功');
            }else{
                console.log('设置失败');
            }
        },
        error: function (status) {
            console.log(status);
            alert('提交失败');
        }
    });
}

//在棋盘上落子并吃子
function LuoAndEatInImg(xy) {
    //落子
    luoziInImg(xy.x, xy.y);
    domusic.play();

    //吃子
    eat(xy.x, xy.y);

    //吃子
    if (ns[0].length !== 0) {
        for (let n of ns[0]) {
            document.getElementById('qizi' + n).remove();
            //不传bu改为传xy后不改变棋谱
            //qizis.splice(n, 1);
            chess[qizis[n].x][qizis[n].y] = 0;
        }
        eatmusic.play();
    }
}

//自适应比例
window.onresize = function () {
    for (let i = 0; i < qizis.length; i++) {
        let xy = qizis.at(i);
        $('#qizi' + i).offset({top: abyToY(xy.y) + qipan.offsetTop - 8, left: abxToX(xy.x) + qipan.offsetLeft - 11})
    }
    jShade().offset({top: shadey + qipan.offsetTop, left: shadex + qipan.offsetLeft});
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
                        LuoAndEatInImg(new XY(abx(e.offsetX), aby(e.offsetY)));
                    } else {
                        errormusic.play();
                    }
                }else{
                    errormusic.play();
                }
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