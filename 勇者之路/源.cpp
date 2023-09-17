#include<graphics.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<conio.h>
#include<Mmsystem.h>
#include<easyx.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#pragma comment(lib,"Winmm.lib")
#define LENGTH 1200
#define WIDTH 700
ExMessage mouse;//声明鼠标
IMAGE actor[4][5], mactor[4], mactorR[4], mactorL[4], mactorB[4],
tishi, bk, bk1, bk2, bk3, cundang, back, cundang1, mb[4], barp, directR, directL,
level1, level1BOSS, gback, mbar, bar1, food1, food2, food3, mweapon, gw[16],wp[16],effect[8],
background[4][2],zhandoukuang,xueliang,mactorshow,jinengstore,jinengp[5],jinbi,jianguang[4],cure_p[3],xuli[3],boom[3],
game1_1,game1_boss,win,fail,kuangnu[3],zhongdup[3],fireball[3],hunluan[3],kuangnuplus[3],game2_boss,newgame;

typedef struct
{
	IMAGE* p;  //图片
	char name[20]; //武器名称
	int atk;  //攻击力
	TCHAR story[30];//详情
}weapon;//武器
weapon w[16];//创建游戏里所有武器的结构体
weapon myweapon[16];//创建游戏存档拥有的武器的结构体
weapon weaponnow;//装备的武器

typedef struct
{
	IMAGE* p;  //图片
	char name[20]; //技能名称
	TCHAR story[30];//详情
	int price;//价格
}jineng;//技能
jineng jinengnow;//装备的技能
jineng jinengall[5];//创建所有技能的结构体
jineng myjineng[5];//存档拥有的技能

int x, y;//人物坐标
int dir=1;//人物方向

int hp = 100;//人物血量
int game_hp;//关卡内血量
char php[10];//打印血量用字符串
int atk = 15;//人物攻击力
int game_atk;//关卡内攻击力
int money = 0;//金钱数量
char pmoney[10];//打印金钱数量
char chardmoney[10];
char cure_count[10];
char yzzl_count[10];

int yzzl = 1;//勇者之力技能指示
int effect_yzzl = 0;
int cure = 1;//治疗指示
int count_cure = 0;
int count_yzzl = 0;
int revive = 0;//可能用到的复活被动判定
int shengguang = 0;//可能用到的圣光判定
int count_shengguang = 0;//圣光读回合数用
int xixue = 0;//可能用到的吸血判定
int fanjia = 0;//可能用到的反甲判定
int hanbin = 0;//可能用到的寒冰判定

int enemy_hp;//敌人血量
int enemy_atk;//敌人攻击力
int enemy_revive;//可能拥有的复活技能
int pkuangnu = 0;//可能拥有的狂怒技能
int pzhongdu = 0;//可能拥有的中毒技能
int phunluan = 0;//可能拥有的混乱技能
int pzhuoshao = 0;//可能拥有的灼烧技能
int xinwang = 0;
int count_hunluan = 0;

int re;//存档编号
int game_levels, zhangjie;//存档需要的一系列变量
int wc, wn,wjineng,newopen;

char patk[10];//打印攻击力用字符串
char pprice[10]; //打印价格用字符串
TCHAR print[30];//打印文本用字符串

int end;//判断游戏结束

void save(int re,int zhangjie, int game_level, int atk, int hp, int weaponnum,int weaponnow,int jinengnow,int money,int newopen)
{
	FILE* file = NULL;//定义文件
	int error;
	if (re == 1)
	{
		error = fopen_s(&file, "存档/存档一.txt", "w");
	}
	if (re == 2)
	{
		error = fopen_s(&file, "存档/存档二.txt", "w");
	}
	if (re == 3)
	{
		error = fopen_s(&file, "存档/存档三.txt", "w");
	}
	fprintf(file, "%d\n", zhangjie);
	fprintf(file, "%d\n", game_level);
	fprintf(file, "%d %d\n",hp,atk);
	fprintf(file, "%d\n", weaponnum);
	fprintf(file, "%d\n", weaponnow);
	fprintf(file, "%d\n", jinengnow);
	fprintf(file, "%d\n", money);
	fprintf(file, "%d\n", newopen);
	fclose(file);
}
int ccount = 0;//人物动作计数
//函数 - char字符串转化为TCHAR字符串↓
void ctot(TCHAR* Tstr, char* str) {

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, str, -1, Tstr, 20);
#else
	strcpy(Tstr, str);
#endif
}
void draw(int x, int y, IMAGE* image, int pic_x = 0, int pic_y = 0, double AA = 1)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();   // GetImageBuffer() 函数，用于获取绘图设备的显存指针， EasyX 自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(image);  // 获取 picture 的显存指针
	int imageWidth = image->getwidth();  // 获取图片宽度
	int imageHeight = image->getheight(); // 获取图片宽度
	int dstX = 0;       // 在 绘图区域 显存里像素的角标
	int srcX = 0;       // 在 image 显存里像素的角标
	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < imageHeight; iy++)
	{
		for (int ix = 0; ix < imageWidth; ix++)
		{
			// 防止越界
			if (ix + pic_x >= 0 && ix + pic_x < imageWidth && iy + pic_y >= 0 && iy + pic_y < imageHeight &&
				ix + x >= 0 && ix + x < LENGTH && iy + y >= 0 && iy + y < WIDTH)
			{
				// 获取像素角标
				int sa = 0, sr = 0, sg = 0, sb = 0;
				int srcX = (ix + pic_x) + (iy + pic_y) * imageWidth;
				dstX = (ix + x) + (iy + y) * LENGTH;

				sa = ((src[srcX] & 0xff000000) >> 24) * AA;   // 0xAArrggbb; AA 是透明度
				sr = ((src[srcX] & 0xff0000) >> 16);    // 获取 RGB 里的 R
				sg = ((src[srcX] & 0xff00) >> 8);     // G
				sb = src[srcX] & 0xff;        // B

				// 设置对应的绘图区域像素信息
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

void initgame()
{
	initgraph(1200, 700);
	loadimage(&mactor[0], L"角色/主角/主角(1).png", 60, 80);
	loadimage(&mactor[1], L"角色/主角/主角(2).png", 60, 80);
	loadimage(&mactor[2], L"角色/主角/主角(3).png", 60, 80);
	loadimage(&mactor[3], L"角色/主角/主角(4).png", 60, 80);
	loadimage(&mactorR[0], L"角色/主角/主角R(1).png", 60, 80);
	loadimage(&mactorR[1], L"角色/主角/主角R(2).png", 60, 80);
	loadimage(&mactorR[2], L"角色/主角/主角R(3).png", 60, 80);
	loadimage(&mactorR[3], L"角色/主角/主角R(4).png", 60, 80);
	loadimage(&mactorL[0], L"角色/主角/主角L(1).png", 60, 80);
	loadimage(&mactorL[1], L"角色/主角/主角L(2).png", 60, 80);
	loadimage(&mactorL[2], L"角色/主角/主角L(3).png", 60, 80);
	loadimage(&mactorL[3], L"角色/主角/主角L(4).png", 60, 80);
	loadimage(&mactorB[0], L"角色/主角/主角B(1).png", 60, 80);
	loadimage(&mactorB[1], L"角色/主角/主角B(2).png", 60, 80);
	loadimage(&mactorB[2], L"角色/主角/主角B(3).png", 60, 80);
	loadimage(&mactorB[3], L"角色/主角/主角B(4).png", 60, 80);
	loadimage(&mactorshow, L"角色/主角/主角B(1).png", 150,200);
	loadimage(&actor[0][0], L"角色/幽暗密林/落魄勇者.png");
	loadimage(&actor[0][1], L"角色/幽暗密林/牛头人.png",350,300);
	loadimage(&actor[0][2], L"角色/幽暗密林/哥布林.png",450,300);
	loadimage(&actor[0][3], L"角色/幽暗密林/古尔丹.png",400,540);
	loadimage(&actor[1][0], L"角色/罪业之都/骷髅.png", 600, 450);
	loadimage(&actor[1][1], L"角色/罪业之都/羽翼骑士.png", 300, 340);
	loadimage(&actor[1][2], L"角色/罪业之都/亚米达拉.png", 1200, 675);
	loadimage(&actor[1][3], L"角色/罪业之都/巨人王尤姆1.png", 1000, 554);
	loadimage(&actor[1][4], L"角色/罪业之都/巨人王尤姆2.png", 1000, 554);
	loadimage(&wp[0], L"武器/板砖.png",44,47);
	loadimage(&wp[1], L"武器/撬棍.png", 44, 47);
	loadimage(&wp[2], L"武器/铁剑.png", 44, 47);
	loadimage(&wp[3], L"武器/金箍棒.png", 44, 47);
	loadimage(&wp[4], L"武器/钻石剑.png", 44, 47);
	loadimage(&wp[5], L"武器/风暴管束者.png", 44, 47);
	loadimage(&wp[6], L"武器/黑暗剑.png", 44, 47);
	loadimage(&wp[7], L"武器/篮球.png", 44, 47);
	loadimage(&wp[8], L"武器/雷神之锤.png", 44, 47);
	loadimage(&wp[9], L"武器/无尽之刃.png", 44, 47);
	loadimage(&wp[10], L"武器/霜之哀伤.png", 44, 47);
	loadimage(&wp[11], L"武器/流亡之刃.png", 44, 47);
	loadimage(&wp[12], L"武器/芝士.png", 44, 47);
	loadimage(&wp[13], L"武器/光剑.png", 44, 47);
	loadimage(&wp[14], L"武器/破坏剑.png", 44, 47);
	loadimage(&wp[15], L"武器/传火大剑.png", 44, 47);
	w[0] = { &wp[0],("板砖"),5,_T("我手里握着的是板砖吗，是天下") };
	w[1] = { &wp[1],("撬棍"),10,_T("物理学圣剑") };
	w[2] = { &wp[2],("铁剑"),15,_T("两个铁锭加一根木棍") };
	w[3] = { &wp[3],("金箍棒"),25,_T("瀑布下的一个洞穴里面找到的") };
	w[4] = { &wp[4],("钻石剑"),20,_T("两个钻石加一根木棍") };
	w[5] = { &wp[5],("风暴管束者"),22,_T("只有风暴才能击倒大树。FNNDP--Mr.Quin") };
	w[6] = { &wp[6],("黑暗剑"),30,_T("黑暗剑22出了吗") };
	w[7] = { &wp[7],("篮球"),5,_T("一个普通的篮球，等等？（要素察觉）") };
	w[8] = { &wp[8],("雷神之锤"),40,_T("北欧的东西怎么在这，这玩意和金箍棒谁重？") };
	w[9] = { &wp[9],("无尽之刃"),45,_T("别急，等我把无尽出了再团") };
	w[10] = { &wp[10],("霜之哀伤"),50,_T("父亲节送礼的好选择") };
	w[11] = { &wp[11],("流亡之刃"),60,_T("奎托斯！!奥林匹斯山的众神都畏惧我！") };
	w[12] = { &wp[12],("芝士"),5,_T("芝士就是力量,我知道你们想让我说雪豹，但我不说") };
	w[13] = { &wp[13],("光剑"),70,_T("原力与你同在？？高科技打冷兵器？") };
	w[14] = { &wp[14],("破坏剑"),75,_T("克劳德什么时候把蒂法介绍给我") };
	w[15] = { &wp[15],("传火大剑"),9999,_T("当火已渐熄，然位不见王影") };
	loadimage(&effect[0], L"特效/中毒.png", 44, 47);
	loadimage(&effect[1], L"特效/虚弱.png", 44, 47);
	loadimage(&effect[2], L"特效/沉默.png", 44, 47);
	loadimage(&effect[3], L"特效/烧伤.png", 44, 47);
	loadimage(&effect[4], L"特效/混沌.png", 44, 47);
	loadimage(&effect[5], L"特效/力量.png", 44, 47);
	loadimage(&effect[6], L"特效/无法行动.png", 44, 47);
	loadimage(&effect[7], L"特效/免疫.png", 44, 47);
	loadimage(&jinengp[0], L"特效/咳血.png", 44, 47);
	loadimage(&jinengp[1], L"特效/荆棘之躯.png", 44, 47);
	loadimage(&jinengp[2], L"特效/圣盾.png", 44, 47);
	loadimage(&jinengp[3], L"特效/寒冬之力.png", 44, 47);
	loadimage(&jinengp[4], L"特效/复活.png", 44, 47);
	loadimage(&jianguang[0], L"特效/剑光/剑光1.png", 140, 180);
	loadimage(&jianguang[1], L"特效/剑光/剑光2.png", 140, 180);
	loadimage(&jianguang[2], L"特效/剑光/剑光3.png", 140, 180);
	loadimage(&jianguang[3], L"特效/剑光/剑光4.png", 140, 180);
	loadimage(&cure_p[0], L"特效/恢复/恢复1.png",128,128);
	loadimage(&cure_p[1], L"特效/恢复/恢复2.png",128,128);
	loadimage(&cure_p[2], L"特效/恢复/恢复3.png",128,128);
	loadimage(&kuangnu[0], L"特效/普通狂怒/狂怒1.png", 128, 128);
	loadimage(&kuangnu[1], L"特效/普通狂怒/狂怒2.png", 128, 128);
	loadimage(&kuangnu[2], L"特效/普通狂怒/狂怒3.png", 128, 128);
	loadimage(&kuangnuplus[0], L"特效/巨人王狂怒/巨人王狂怒1.png", 128, 128);
	loadimage(&kuangnuplus[1], L"特效/巨人王狂怒/巨人王狂怒2.png", 128, 128);
	loadimage(&kuangnuplus[2], L"特效/巨人王狂怒/巨人王狂怒3.png", 128, 128);
	loadimage(&xuli[0], L"特效/蓄力/蓄力1.png", 128, 128);
	loadimage(&xuli[1], L"特效/蓄力/蓄力2.png", 128, 128);
	loadimage(&xuli[2], L"特效/蓄力/蓄力3.png", 128, 128);
	loadimage(&boom[0], L"特效/爆炸/爆炸1.png", 128, 128);
	loadimage(&boom[1], L"特效/爆炸/爆炸2.png", 128, 128);
	loadimage(&boom[2], L"特效/爆炸/爆炸3.png", 128, 128);
	loadimage(&zhongdup[0], L"特效/毒气/毒气1.png", 128, 128);
	loadimage(&zhongdup[1], L"特效/毒气/毒气2.png", 128, 128);
	loadimage(&zhongdup[2], L"特效/毒气/毒气3.png", 128, 128);
	loadimage(&fireball[0], L"特效/火球/火球1.png", 128, 128);
	loadimage(&fireball[1], L"特效/火球/火球2.png", 128, 128);
	loadimage(&fireball[2], L"特效/火球/火球3.png", 128, 128);
	loadimage(&hunluan[0], L"特效/混乱/混乱1.png", 128, 128);
	loadimage(&hunluan[1], L"特效/混乱/混乱2.png", 128, 128);
	loadimage(&hunluan[2], L"特效/混乱/混乱3.png", 128, 128);
	jinengall[0] = { &jinengp[0],("咳血"),_T("在你攻击敌人时会获得一定程度治疗") ,100};
	jinengall[1] = { &jinengp[1],("荆棘之躯"),_T("在敌人攻击你时会受到一定程度伤害") ,200};
	jinengall[2] = { &jinengp[2],("圣光"),_T("每2回合获得一次关卡内属性增强") ,300};
	jinengall[3] = { &jinengp[3],("寒冬之力"),_T("拥有寒冰的力量，仅对特定章节有用"),500 };
	jinengall[4] = { &jinengp[4],("复活"),_T("一次挑战只能施法一次，角色死亡后复活"),1000 };
	loadimage(&jinbi, L"背景/金币页面.png");
	loadimage(&zhandoukuang, L"背景/技能框.png");
	loadimage(&xueliang, L"背景/血量.png");
	loadimage(&win, L"背景/胜利.png");
	loadimage(&fail, L"背景/失败.png");
	loadimage(&background[0][0], L"背景/第一关/小怪.jpg", 1200, 700);
	loadimage(&background[0][1], L"背景/第一关/BOSS.jpg", 1200, 700);
	loadimage(&background[1][0], L"背景/第二关/小怪.jpg", 1200, 700);
	loadimage(&background[1][1], L"背景/第二关/BOSS.jpg", 1200, 700);
	loadimage(&game1_1, L"背景/第一关/第一关对话界面.jpg", 1200, 700);
	loadimage(&game1_boss, L"背景/第一关/BOSS对话界面.jpg");
	loadimage(&game2_boss, L"背景/第二关/BOSS对话界面.jpg");
	loadimage(&newgame, L"背景/剧情介绍.png",1200,700);
}

void success(int zhang,int jie)
{
	mciSendString(L"close BGM", 0, 0, 0);
	PlaySound(L"音乐/胜利.wav", 0, SND_ASYNC);
	draw(0,0, &win);
	if (zhang == 1 && jie == 3 && weaponnow.atk!=22)
	{
		outtextxy(100, 600, _T("只有风暴才能击倒大树？FNNDP"));
	}
	FlushBatchDraw();
	Sleep(8000);
	if (zhang == zhangjie-1 && jie == game_levels-1)
	{
		if (game_levels == 4)
		{
			zhangjie += 1;
			game_levels = 1;
		}
		else
		{
			game_levels += 1;
		}
	}
	return;
}

void defeat(int zhang,int jie)
{
	mciSendString(L"close BGM", 0, 0, 0);
	PlaySound(L"音乐/失败.wav", 0, SND_ASYNC);
	draw(0, 0, &fail);
	if (zhang == 1 && jie == 3&&weaponnow.atk!=22)
	{
		outtextxy(300, 600, _T(" 只有风暴才能击倒大树"));
	}
	FlushBatchDraw();
	Sleep(5000);
	return;
}

void fightshow(int zhang,int jie,int changjing)
{
	putimage(0, 0, &background[zhang][changjing]);//背景
	if (zhang == 0 && jie == 3)
	{
		draw(400, 0, &actor[zhang][jie]);//古尔丹
	}
	else
	{
		if (zhang == 1 && jie == 2)
		{
			draw(0, -180, &actor[zhang][jie]);//亚米达拉
		}
		else
		{
			if (zhang == 1 && jie == 3 && xinwang == 0)
			{
				draw(70, -20, &actor[zhang][jie]);//尤姆1
			}
			else
			{
				if (zhang == 1 && jie == 3 && xinwang == 1)
				{
					draw(70, -20, &actor[zhang][jie+1]);//尤姆2
				}
				else
				{
					draw(500, 100, &actor[zhang][jie]);//普通敌人
				}
			}
		}
	}
	draw(0, 0, &zhandoukuang);//战斗框
	draw(0, 0, &xueliang);//血量图片
	draw(500, 500, &mactorshow);//玩家
	_itoa_s(game_hp, php, 10);
	ctot(print, php);
	outtextxy(160, 530, print);//血量
	_itoa_s(game_atk, patk, 10);
	ctot(print, patk);
	outtextxy(160, 580, print);//攻击力
	_itoa_s(enemy_hp, php, 10);
	ctot(print, php);
	outtextxy(160, 40, print);//敌人血量
	draw(1050, 635, jinengnow.p);//携带的被动技能
	if (pkuangnu == 1)
	{
		draw(150, 100, &effect[5]);
	}
	if (pzhongdu != 0)
	{
		draw(150, 650, &effect[0]);
	}
	if (pzhuoshao != 0)
	{
		draw(100, 650, &effect[3]);
	}
	FlushBatchDraw();
}

int jinengshifang()
{
	if (mouse.lbutton == 1 && (mouse.x >= 817 && mouse.x <= 1010) && (mouse.y >= 506 && mouse.y <= 605))//普通攻击
	{
		return 1;
	}
	if (mouse.lbutton == 1 && yzzl == 0 && (mouse.x >= 1020 && mouse.x <= 1200) && (mouse.y >= 506 && mouse.y <= 605))//勇者之力
	{
		return 2;
	}
	if (mouse.lbutton == 1 && cure == 0 && (mouse.x >= 817 && mouse.x <= 1010) && (mouse.y >= 615 && mouse.y <= 700))//治疗
	{
		return 3;
	}
	return 0;
}

void playerfight(int *player,int*flag,int *zhang,int jie,int changjing)
{
	LOGFONT f;//声明字体样式变量
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("方正姚体"));
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	peekmessage(&mouse, EM_MOUSE);
	switch (mouse.message)
	{
	case WM_LBUTTONDOWN:
	{
		*player = jinengshifang();//读取玩家选择
		flushmessage(EM_MOUSE);
		//判断玩家行为↓
		if (*player == 1)//普通攻击
		{
			mouse.lbutton = NULL;
			*flag = 1;
			if (*zhang == 1 && jie == 3&&weaponnow.atk==22)
			{
				PlaySound(L"音效/暴风大剑.wav", 0, SND_ASYNC);
				Sleep(2000);
			}
			else
			{
				PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
			}
			Sleep(100);
			for (int t = 0; t < 4; t++)
			{
				draw(550, 150, &jianguang[t]);
				FlushBatchDraw();
				Sleep(50);
			}
			enemy_hp -= game_atk;

			if (xixue == 1)
			{
				game_hp += game_atk / 5;
			}
			if (phunluan == 1)
			{
				phunluan = 0;
				srand((unsigned int)time(NULL));//敌人行为
				int bh_2 = rand() % 100 + 1;
				if (bh_2 <= count_hunluan * 10)
				{
					PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
					game_hp -= game_atk;
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close BGM", 0, 0, 0);
							defeat(*zhang,jie);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							f.lfHeight = 20;
							outtextxy(0, 500, _T("重生触发"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(700);
							game_hp = hp;
							revive = 0;
							fightshow(*zhang, jie, changjing);
						}
					}
				}
			}


			fightshow(*zhang,jie,changjing);
			if (enemy_hp <= 0&&enemy_revive==0)
			{
				if (*zhang == 0 && jie == 3)
				{
					mciSendString(L"close 音乐/魔兽世界.mp3", 0, 0, 0);
				}
				else
				{
					mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
				}
				end = 1;
				int dmoney = 200 * (jie + 1);
				money += dmoney;
				_itoa_s(dmoney, chardmoney, 10);
				ctot(print, chardmoney);
				outtextxy(560, 390, print);
				FlushBatchDraw();
				if (wc == ((*zhang) * 4 + jie + 1))
				{
					wc += 1;
				}
				success(*zhang,jie);
				save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
				return;
			}
			if (enemy_hp <= 0 && enemy_revive == 1)
			{
				PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
				enemy_hp = 800;
				enemy_revive = 0;
				fightshow(*zhang, jie, changjing);
			}
			Sleep(250);
		}
		if (*player == 2)//勇者之力
		{
			count_yzzl = 0;
			mouse.lbutton = NULL;
			yzzl = 1;
			*flag = 1;
			PlaySound(L"音效/蓄力.wav", 0, SND_ASYNC);
			Sleep(50);
			for (int t = 0; t < 3; t++)
			{
				draw(360, 400, &xuli[t]);
				FlushBatchDraw();
				Sleep(80);
			}
			Sleep(500);
			PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
			for (int t = 0; t < 3; t++)
			{
				draw(550, 150, &boom[t]);
				FlushBatchDraw();
				Sleep(80);
			}
			enemy_hp -= 2 * game_atk;
			if (xixue == 1)
			{
				game_hp += game_atk;
			}
			effect_yzzl = 1;
			if (enemy_hp <= 0&&enemy_revive==0)
			{
				if (*zhang == 0 && jie == 3)
				{
					mciSendString(L"close 音乐/魔兽世界.mp3", 0, 0, 0);
				}
				else
				{
					mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
				}
				end = 1;
				int dmoney = 200 * (jie + 1);
				money += dmoney;
				_itoa_s(dmoney, chardmoney, 10);
				ctot(print, chardmoney);
				outtextxy(560, 390, print);
				FlushBatchDraw();
				if (wc == ((*zhang) * 4 + jie + 1))
				{
					wc += 1;
				}
				success(*zhang,jie);
				save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
				return;
			}
			if (enemy_hp <= 0 && enemy_revive == 1)
			{
				PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
				enemy_hp = 800;
				enemy_revive = 0;
				fightshow(*zhang, jie, changjing);
			}
			f.lfHeight = 20;
			outtextxy(0, 500, _T("你使用了勇者之力，将无视对方下回合的行动"));
			f.lfHeight = 45;
			FlushBatchDraw();
			Sleep(700);
		}
		if (*player == 3)
		{
			count_cure = 0;
			mouse.lbutton = NULL;
			PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
			cure = 1;
			*flag = 1;
			if (game_hp < hp)
			{
				game_hp += (hp - game_hp) / 3;//恢复已损失生命值的三分之一
			}
			for (int i = 0; i < 3; i++)
			{
				draw(360, 400, &cure_p[i]);
				FlushBatchDraw();
				Sleep(50);
			}
			f.lfHeight = 20;
			outtextxy(0, 500, _T("你恢复了已损失血量的1/3"));
			f.lfHeight = 45;
			FlushBatchDraw();
			Sleep(700);
		}
		Sleep(200);
		fightshow(*zhang,jie,changjing);
	}
	}
}

void ctolgame()//人物控制
{
	if (_kbhit())
	{
		int ch = _getch();
		switch (ch)
		{
		case 's': dir = 1, y += 18;
			break;
		case 'a': dir = 2,x-=18;
			break;
		case 'd': dir = 3,x+=18;
			break;
		case 'w': dir = 4,y-=18;
			break;
		}
	}
}

void zhujue()//主角贴图
{
	switch (dir)
	{
	case 1:
		draw(x, y, &mactor[ccount]);
		ccount++;
		if (ccount == 4)
		{
			ccount = 0;
		}
		break;
	case 2:
		draw(x, y, &mactorR[ccount]);
		ccount++;
		if (ccount == 4)
		{
			ccount = 0;
		}
		break;
	case 3:
		draw(x, y, &mactorL [ccount] );
		ccount++;
		if (ccount == 4)
		{
			ccount = 0;
		}
		break;
	case 4:
		draw(x, y, &mactorB[ccount]);
		ccount++;
		if (ccount == 4)
		{
			ccount = 0;
		}
		break;
	}
}

int menu()
{
	IMAGE statement;
	int state = 0, start = 0;

	LOGFONT f;//声明字体样式变量
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("方正姚体"));
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	f.lfHeight = 60;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	loadimage(&bk, L"背景/菜单.jpg", 1200, 700);
	loadimage(&cundang, L"背景/存档界面.jpg", 1200, 700);
	loadimage(&back, L"背景/返回按钮.png");
	loadimage(&statement, L"背景/声明.png", 1200, 700);

	mciSendString(L"open 音乐/菜单.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

	while (1)
	{
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, &bk);
		while (peekmessage(&mouse, EM_MOUSE))
		{
			switch (mouse.message)
			{
			case WM_LBUTTONDOWN:
				if ((mouse.x > 443 && mouse.x < 770) && (mouse.y > 404 && mouse.y < 451))
				{
					flushmessage(EM_MOUSE);
					mouse.lbutton = NULL;
					start = 1;
					Sleep(200);
				}
				if ((mouse.x > 443 && mouse.x < 770) && (mouse.y > 508 && mouse.y < 529))
				{
					flushmessage(EM_MOUSE);
					mouse.lbutton = NULL;
					state = 1;
					Sleep(200);
				}
				break;
			}
		}
		if (state)
		{
			peekmessage(&mouse, EM_MOUSE);
			f.lfHeight = 50;
			settextstyle(&f);
			setfillcolor(WHITE);
			settextcolor(BLACK);
			putimage(0, 0, &statement);
			outtextxy(60, 40, _T("声明："));
			outtextxy(60, 350, _T("制作人："));
			outtextxy(60, 270, _T("特别鸣谢："));
			outtextxy(60, 530, _T("操作说明："));
			f.lfHeight = 40;
			settextstyle(&f);
			outtextxy(260, 360, _T("UESTC-22级xxx"));
			outtextxy(260, 280, _T(" 各视频网站和论坛分享经验的UP主和博主"));
			outtextxy(260, 540, _T("酒馆WASD移动，J进入光圈，其余鼠标操作"));
			f.lfHeight = 30;
			settextstyle(&f);
			outtextxy(60, 100, _T("背景音乐来自《冒险岛》,《地下城与勇士》，《怪物猎人》等经典游戏"));
			outtextxy(60, 140, _T("战斗机制灵感来源上世纪的JRPG，因时间和一个人制作的原因只完成了前两章"));
			outtextxy(60, 180, _T("贴图除少量自制外其余来源于网络分享, 鸣谢所有作者。"));
			outtextxy(60, 220, _T("本游戏无任何商业用途，向所有游戏作者致敬！"));
			peekmessage(&mouse, EM_MOUSE);
			if (mouse.lbutton)
			{
				state = 0;
				mouse.lbutton = NULL;
				Sleep(50);
			}
		}
		if (start)
		{
			putimage(0, 0, &cundang);//存档页面背景图
			settextcolor(WHITE);
			peekmessage(&mouse, EM_MOUSE);
			if ((mouse.x > 150 && mouse.x < 400) && (mouse.y > 250 && mouse.y < 470))
			{
				if (mouse.lbutton)
				{
					mouse.lbutton = NULL;
					return 1;
				}
			}
			if ((mouse.x > 470 && mouse.x < 720) && (mouse.y > 250 && mouse.y < 470))
			{
				if (mouse.lbutton)
				{
					mouse.lbutton = NULL;
					return 2;
				}
			}
			if ((mouse.x > 790 && mouse.x < 1040) && (mouse.y > 250 && mouse.y < 470))
			{
				if (mouse.lbutton)
				{
					mouse.lbutton = NULL;
					return 3;
				}
			}
			if ((mouse.x > 1100 && mouse.x < 1200) && (mouse.y > 620 && mouse.y < 700))
			{
				if (mouse.lbutton)
				{
					mouse.lbutton = NULL;
					start = 0;
					Sleep(50);
				}
			}
		}
		EndBatchDraw();
		Sleep(100);
	}
}

void game_level(int* pn)
{
	TCHAR str[6];//字符数组
	swprintf_s(str, _T("%d"), *pn+1);
	draw((*pn+1) * 200, 350 + 150 * pow(-1, *pn), &level1);//图标
	outtextxy((*pn) *200+220, 355 + 150 * pow(-1, *pn), str);//序号
	(*pn)++;
}

void jiuguan(int* c)
{

	x = 570, y = 600;
	dir = 1;
	int bar = 0;
	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open 音乐/酒馆.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 500", NULL, 0, NULL);
	loadimage(&mbar, L"背景/酒馆.jpg", 1200, 700);
	loadimage(&bar1, L"背景/吧台菜单.png", 900, 175);
	loadimage(&food1, L"背景/猫饭.jpg", 180, 100);
	loadimage(&food2, L"背景/萨维奈奶茶.png", 180, 100);
	loadimage(&mweapon, L"背景/装备栏.png");
	loadimage(&jinengstore, L"背景/技能商店.png");
	cleardevice();
	int pjiuguan = 0;//指示退出酒馆用变量
	while (pjiuguan==0)
	{
		LOGFONT f;//声明字体样式变量
		gettextstyle(&f);
		wcscpy_s(f.lfFaceName, _T("方正姚体"));
		f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
		f.lfHeight = 32;
		settextstyle(&f);
		setbkmode(TRANSPARENT);
		setlinestyle(PS_SOLID, 3);
		setfillcolor(WHITE);
		settextstyle(&f);

		int h = 0;
		bar = 0;
		peekmessage(&mouse, EM_MOUSE);
		BeginBatchDraw();
		putimage(0, 0, &mbar);//酒馆背景
		ctolgame();//人物控制
		zhujue();//主角贴图
		if ((x >= 757 && x <= 805) && (y >= 426 && y <= 446))//武器事件
		{
			char kbar = _getch();
			if (kbar == 'j')
			{
				while (bar == 0)
				{
					putimage(0, 0, &mbar);
					draw(200, 116, &mweapon);
					draw(900, 645, &gback);
					draw(0, 0, &jinbi);
					_itoa_s(money, pmoney, 10);
					ctot(print, pmoney);
					outtextxy(1020, 40, print);
					if (h == 4)
					{
						h = 0;
					}
					draw(344, 192, &mactor[h]);
					h++;
					draw(452, 191, weaponnow.p);
					draw(452, 240, jinengnow.p);
					for (int i = 0; i < 4; i++)//i为行数
					{
						for (int j = 0; j < 4; j++)//j为列数
						{
							if ((4 * i + j)<wc)
							{
								draw(665+j*65,181+i*71,w[4 * i + j].p);
							}
						}
					}
					for (int i = 0; i < 4; i++)//i为行数
					{
						for (int j = 0; j < 4; j++)//j为列数
						{
							if (mouse.x >= 665 + j * 65 && mouse.x <= 709 + j * 65 && mouse.y >= 181 + i * 71 && mouse.y <= 228 + i * 71&&4 * i + j<wc)
							{
								ctot(print, w[4 * i + j].name);
								f.lfHeight = 15;
								outtextxy(331, 326, print);
								_itoa_s(w[4 * i + j].atk, patk, 10);
								ctot(print, patk);
								f.lfHeight = 5;
								outtextxy(331, 376, _T("攻击力："));
								outtextxy(431, 376, print);
								outtextxy(331, 426, w[4 * i + j].story);
							}
						}
					}
					while (peekmessage(&mouse, EM_MOUSE))
					{
							switch (mouse.message)
							{
							case WM_LBUTTONDOWN:
							case WM_LBUTTONUP:
							{
								if ((mouse.x > 930 && mouse.x < 1060) && (mouse.y > 645 && mouse.y < 700))
								{
									bar = 1;
									mouse.lbutton = NULL;
									x = 757; y = 470;
									break;
								}
								for (int i = 0; i < 4; i++)//i为行数
								{
									for (int j = 0; j < 4; j++)//j为列数
									{
										if (mouse.x >= 665 + j * 65 && mouse.x <= 709 + j * 65 && mouse.y >= 181 + i * 71 && mouse.y <= 228 + i * 71&& 4 * i + j<wc)
										{
											atk -= weaponnow.atk;
											weaponnow = w[4 * i + j];
											atk += weaponnow.atk;
											wn = 4 * i + j;
											save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//存档
										}
									}
								}
							}
							}
					}
					FlushBatchDraw();
					Sleep(200);
				}
			}
		}
		if ((x >= 201 && x <= 248) && (y >= 191 && y <= 211))//吧台事件
		{
			char kbar = _getch();
			if (kbar == 'j')
			{
				while (bar == 0)
				{
					putimage(0, 0, &mbar);
					draw(157, 525, &bar1);
					draw(900, 645, &gback);
					draw(0, 0, &jinbi);
					_itoa_s(money, pmoney, 10);
					ctot(print, pmoney);
					outtextxy(1020, 40, print);
					putimage(260,540,&food1);
					outtextxy(310,650, _T("猫饭"));
					putimage(520, 540, &food2);
					outtextxy(530, 650, _T("萨维奈奶茶"));
					outtextxy(730, 580, _T("300金币购买"));
					while (peekmessage(&mouse, EM_MOUSE))
					{
						switch (mouse.message)
						{
						case WM_LBUTTONDOWN:
						{
							if ((mouse.x > 930 && mouse.x < 1060) && (mouse.y > 645 && mouse.y < 700))
							{
								bar = 1;
								mouse.lbutton = NULL;
								x = 201; y = 230;
								break;
							}
							if (money>=300&&(mouse.x > 261 && mouse.x < 439) && (mouse.y > 541 && mouse.y < 637))
							{
								putimage(300, 175, &tishi);
								outtextxy(550, 250, _T("购买成功！"));
								outtextxy(550, 350, _T("攻击力+10"));
								atk += 10;
								money -= 300;
								save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//存档
								FlushBatchDraw();
							}
							if (money >= 300 && (mouse.x > 520 && mouse.x < 700) && (mouse.y > 541 && mouse.y < 637))
							{
								putimage(300, 175, &tishi);
								outtextxy(550, 250, _T("购买成功！"));
								outtextxy(550, 350, _T("生命值+100"));
								hp += 100;
								money -= 300;
								save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//存档
								FlushBatchDraw();
							}
						}
						default:continue;
						}
					}
					FlushBatchDraw();
					Sleep(200);
				}
			}
		}
		if ((x >= 261 && x <= 311) && (y >= 600 && y <= 620))//图册事件
		{
			char kbar = _getch();
			if (kbar == 'j')
			{
				while (bar == 0)
				{
					putimage(0, 0, &mbar);
					draw(200, 116, &jinengstore);
					draw(900, 645, &gback);
					draw(0, 0, &jinbi);
					_itoa_s(money, pmoney, 10);
					ctot(print, pmoney);
					outtextxy(1020, 40, print);
					if (h == 4)
					{
						h = 0;
					}
					draw(344, 192, &mactor[h]);
					h++;
					draw(452, 170, weaponnow.p);
					draw(452, 220, jinengnow.p);
					for (int i = 0; i < 5; i++)
					{
						draw(664, 160 + i * 65, &jinengp[i]);
						_itoa_s(jinengall[i].price, pprice, 10);
						ctot(print, pprice);
						outtextxy(820, 160 + i * 65, print);
					}
					for (int j = 0; j < 5; j++)//j为列数
					{
						if (mouse.x >= 664 && mouse.x <= 708 && mouse.y >= 160 + j * 65 && mouse.y <= 207 + j * 65)
						{
							ctot(print, jinengall[j].name);
							f.lfHeight = 15;
							outtextxy(331, 326, print);
							f.lfHeight = 5;
							outtextxy(331, 426, jinengall[j].story);
						}
					}
					while (peekmessage(&mouse, EM_MOUSE))
					{
						switch (mouse.message)
						{
						case WM_LBUTTONDOWN:
						{
							if ((mouse.x > 930 && mouse.x < 1060) && (mouse.y > 645 && mouse.y < 700))
							{
								bar = 1;
								mouse.lbutton = NULL;
								x = 330; y = 610;
								break;
							}
							for (int j = 0; j < 5; j++)
								{
									if (mouse.x >= 664 && mouse.x <= 708 && mouse.y >= 160 + j * 65 && mouse.y <= 207 + j * 65)
									{
										if (money >= jinengall[j].price)
										{
											money = money - jinengall[j].price;
											jinengnow = jinengall[j];
											wjineng = j;
											save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//存档
										}
									}
								}
						}
						}
					}
					FlushBatchDraw();
					Sleep(200);
				}
			}
		}
		while (peekmessage(&mouse, EM_MOUSE))
		{
			switch (mouse.message)
			{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				if ((mouse.x > 1100 && mouse.x < 1200) && (mouse.y > 620 && mouse.y < 700))//返回章节页面
				{
					mouse.lbutton = 0;
					pjiuguan = 1;
				}
				break;
			default:continue;
			}
		}
		EndBatchDraw();
		Sleep(160);
	}
	mciSendString(L"close 音乐/酒馆.mp3", 0, 0, 0);
	switch (*c)
	{
	case 0:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	case 1:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open 音乐/黑暗之魂3.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	case 2:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open 音乐/暗黑破坏神3.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	case 3:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open 音乐/最终.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	}
	return;
}

void game1(int zhang)//关卡1
{
	cleardevice();
	flushmessage(EM_MOUSE);

	LOGFONT f;//声明字体样式变量
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("方正姚体"));
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open 音乐/魂斗罗.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

	int player, enemy;//判断玩家和敌人的行为
	game_atk = atk;
	game_hp = hp;
	int flag;//判断玩家是否操作
	yzzl = 0;//勇者之力技能指示
	effect_yzzl = 0;//勇者之力效果判断
	cure = 0;//治疗指示
	count_cure = 0;
	pzhuoshao = 0;
	count_yzzl = 0;
	count_shengguang = 0;
	pkuangnu = 0;
	end = 0;
	phunluan = 0;
	count_hunluan = 0;
	pzhongdu = 0;
	enemy_revive = 0;
	//被动技能判断↓先默认全部关闭，若携带则打开
	revive = 0;
	if (jinengnow.price == 1000)
	{
		revive = 1;
	}
	shengguang = 0;
	if (jinengnow.price == 300)
	{
		shengguang = 1;
	}
	fanjia = 0;
	if (jinengnow.price == 200)
	{
		fanjia = 1;
	}
	hanbin = 0;
	if (jinengnow.price == 500)
	{
		hanbin = 1;
	}
	xixue = 0;
	if (jinengnow.price == 100)
	{
		xixue = 1;
	}

	int bh;//敌人行为随机数

	switch (zhang)
	{
	case 0://关卡1-1
	{
		end = 0;
		enemy_hp = 200;
		enemy_atk = 15;
		putimage(0, 0, &game1_1);
		outtextxy(100, 600, _T("勇者：你不知道你做出了什么选择,我不能让你去冒险"));
		FlushBatchDraw();
		Sleep(3000);
		while (end != 1)
		{
			flag = 0;

			fightshow(zhang, 0, 0);

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 80)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
					Sleep(100);
					for (int t = 0; t < 4; t++)
					{
						draw(550, 550, &jianguang[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					game_hp -= enemy_atk;
					if (fanjia == 1)
					{
						PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							end = 1;
							int dmoney = 200 * 2;
							money += dmoney;
							_itoa_s(dmoney, chardmoney, 10);
							ctot(print, chardmoney);
							outtextxy(560, 390, print);
							FlushBatchDraw();
							if (wc == 2)
							{
								wc += 1;
							}
							success(1, 1);
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money, newopen);//存档
							return;
						}
						fightshow(zhang, 0, 0);
						if (game_hp <= 0)
						{
							if (revive != 1)
							{
								mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
								defeat(zhang, 0);
								end = 1;
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money, newopen);//存档
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
								game_hp = hp;
								revive = 0;
								fightshow(zhang, 0, 0);
							}
						}
						fightshow(zhang, 0, 0);
					}
				}
					if (bh > 80)
					{
						mouse.lbutton = NULL;
						PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
						if (enemy_hp < 200)
						{
							enemy_hp += (200 - enemy_hp) / 3;//恢复已损失生命值的三分之一
						}
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &cure_p[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						f.lfHeight = 20;
						outtextxy(0, 500, _T("勇者恢复了一定生命值"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);
						fightshow(zhang, 0, 0);
					}
				Sleep(200);
				fightshow(zhang, 0, 0);
				FlushBatchDraw();

				count_shengguang++;
				if (shengguang == 1 && count_shengguang % 2 == 0)
				{
					PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
					game_hp += 10;
					game_atk += 10;
					fightshow(zhang, 0, 0);
				}

				if (count_cure != 5 && cure == 1)
				{
					_itoa_s(5 - count_cure, cure_count, 10);
					ctot(print, cure_count);
					outtextxy(1005, 635, print);
				}
				if (count_yzzl != 5 && yzzl == 1)
				{
					_itoa_s(5 - count_yzzl, yzzl_count, 10);
					ctot(print, yzzl_count);
					outtextxy(1005, 530, print);
				}
				FlushBatchDraw();
				mouse.lbutton = NULL;
				while (flag == 0)
				{
					Sleep(5);
					playerfight(&player, &flag, &zhang, 0, 0);
				}
			}
			Sleep(250);
		}
		break;
	}
	case 1://关卡2-1
	{
		enemy_hp = 600;
		enemy_atk = 60;
		enemy_revive = 1;

		while (end != 1)
		{
			flag = 0;

			fightshow(zhang, 0, 0);

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 80)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
					Sleep(100);
					for (int t = 0; t < 4; t++)
					{
						draw(550, 550, &jianguang[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					game_hp -= enemy_atk;
					if (fanjia == 1)
					{
						PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("反甲对骷髅无效"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);

					}
					fightshow(zhang, 0, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,0);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 0, 0);
						}
					}
				}
				if (bh > 80)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"音效/普通狂怒.wav", 0, SND_ASYNC);
					enemy_atk += 20;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("骷髅伤害提高了"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 0, 0);
				}
			}
				Sleep(500);
				FlushBatchDraw();

				count_shengguang++;
				if (shengguang == 1 && count_shengguang % 2 == 0)
				{
					PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
					game_hp += 10;
					game_atk += 10;
					fightshow(zhang, 0, 0);
				}

				if (count_cure != 5 && cure == 1)
				{
					_itoa_s(5 - count_cure, cure_count, 10);
					ctot(print, cure_count);
					outtextxy(1005, 635, print);
				}
				if (count_yzzl != 5 && yzzl == 1)
				{
					_itoa_s(5 - count_yzzl, yzzl_count, 10);
					ctot(print, yzzl_count);
					outtextxy(1005, 530, print);
				}
				FlushBatchDraw();
				mouse.lbutton = NULL;
				while (flag == 0)
				{
					Sleep(5);
					playerfight(&player, &flag, &zhang, 0, 0);
				}
			Sleep(250);
		}
		break;
	}
	case 2://关卡3-1
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	case 3://关卡4-1
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	}
}

void game2(int zhang)
{
	cleardevice();
	flushmessage(EM_MOUSE);

	LOGFONT f;//声明字体样式变量
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("方正姚体"));
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open 音乐/魂斗罗.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

	int player, enemy;//判断玩家和敌人的行为
	game_atk = atk;
	game_hp = hp;
	int flag;//判断玩家是否操作
	yzzl = 0;//勇者之力技能指示
	effect_yzzl = 0;//勇者之力效果判断
	cure = 0;//治疗指示
	count_cure = 0;
	count_yzzl = 0;
	count_shengguang = 0;
	pkuangnu = 0;
	pzhuoshao = 0;
	pkuangnu = 0;
	enemy_revive = 0;
	pzhongdu = 0;
	end = 0;
	//被动技能判断↓先默认全部关闭，若携带则打开
	revive = 0;
	if (jinengnow.price == 1000)
	{
		revive = 1;
	}
	shengguang = 0;
	if (jinengnow.price == 300)
	{
		shengguang = 1;
	}
	fanjia = 0;
	if (jinengnow.price == 200)
	{
		fanjia = 1;
	}
	hanbin = 0;
	if (jinengnow.price == 500)
	{
		hanbin = 1;
	}
	xixue = 0;
	if (jinengnow.price == 100)
	{
		xixue = 1;
	}

	int bh;//敌人行为随机数

	switch (zhang)
	{
	case 0:
	{
		enemy_hp = 400;
		enemy_atk = 20;
		while (end != 1)
		{
			flag = 0;

			fightshow(zhang,1,0);

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 70)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
					Sleep(100);
					for (int t = 0; t < 4; t++)
					{
						draw(550, 550, &jianguang[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					game_hp -= enemy_atk;
					if (fanjia == 1)
					{
						PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							end = 1;
							int dmoney = 200 * 2;
							money += dmoney;
							_itoa_s(dmoney, chardmoney, 10);
							ctot(print, chardmoney);
							outtextxy(560, 390, print);
							FlushBatchDraw();
							if (wc == 2)
							{
								wc += 1;
							}
							success(0, 1);
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
					}
					fightshow(zhang,1,0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,1);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang,1,0);
						}
					}
				}
				if (bh > 90&&bh<=100)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
					if (enemy_hp < 300)
					{
						enemy_hp +=(300-enemy_hp)/3;//恢复已损失生命值的三分之一
					}
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &cure_p[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("牛头人恢复了一定的生命值"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang,1,0);
				}
				if (bh > 70 && bh <= 90)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"音效/普通狂怒.wav", 0, SND_ASYNC);
					enemy_atk += 10;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("牛头人伤害提高了"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 1,0);
				}
			}
			Sleep(500);
			FlushBatchDraw();

			count_shengguang++;
			if (shengguang == 1 && count_shengguang % 2 == 0)
			{
				PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				fightshow(zhang,1,0);
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();
			mouse.lbutton = NULL;
			while (flag == 0)
			{
				Sleep(5);
				playerfight(&player, &flag, &zhang,1,0);
			}
		}
		Sleep(250);
		break;
	}
	case 1:
	{
		enemy_hp = 1200;
		enemy_atk = 40;
		while (end != 1)
		{
			flag = 0;

			fightshow(zhang, 1, 0);

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 50)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
					Sleep(100);
					for (int t = 0; t < 4; t++)
					{
						draw(550, 550, &jianguang[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					game_hp -= enemy_atk;
					if (fanjia == 1)
					{
						PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							end = 1;
							int dmoney = 200 * 2;
							money += dmoney;
							_itoa_s(dmoney, chardmoney, 10);
							ctot(print, chardmoney);
							outtextxy(560, 390, print);
							FlushBatchDraw();
							if (wc == 2)
							{
								wc += 1;
							}
							success(0, 1);
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
					}
					fightshow(zhang, 1, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,1);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 1, 0);
						}
					}
				}
				if (bh > 50&&bh<=70)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
					Sleep(100);
					for (int j = 0; j < 3; j++)
					{
						for (int t = 0; t < 4; t++)
						{
							draw(550, 550, &jianguang[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						fightshow(zhang, 1, 0);
					}
					game_hp -= enemy_atk*3;
					if (fanjia == 1)
					{
						PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							end = 1;
							int dmoney = 200 * 2;
							money += dmoney;
							_itoa_s(dmoney, chardmoney, 10);
							ctot(print, chardmoney);
							outtextxy(560, 390, print);
							FlushBatchDraw();
							if (wc == 2)
							{
								wc += 1;
							}
							success(0, 1);
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
					}
					f.lfHeight = 20;
					outtextxy(0, 500, _T("羽翼骑士使用了旋风斩,造成了3段打击"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(500);
					fightshow(zhang, 1, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,1);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 1, 0);
						}
					}
				}
				if (bh > 85 && bh <= 100)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
					if (enemy_hp < 1200)
					{
						enemy_hp += 400;//恢复已损失生命值的三分之一
					}
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &cure_p[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("羽翼骑士恢复了一定的生命值"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 1, 0);
				}
				if (bh > 70 && bh <= 85)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"音效/普通狂怒.wav", 0, SND_ASYNC);
					enemy_atk += 15;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("羽翼骑士伤害提高了"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 1, 0);
				}
			}
			Sleep(500);
			FlushBatchDraw();

			count_shengguang++;
			if (shengguang == 1 && count_shengguang % 2 == 0)
			{
				PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				fightshow(zhang, 1, 0);
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();
			mouse.lbutton = NULL;
			while (flag == 0)
			{
				Sleep(5);
				playerfight(&player, &flag, &zhang, 1, 0);
			}
		}
		Sleep(250);
		break;
	}
	case 2:
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	case 3:
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	}
}

void game3(int zhang)
{
	cleardevice();
	flushmessage(EM_MOUSE);

	LOGFONT f;//声明字体样式变量
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("方正姚体"));
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open 音乐/魂斗罗.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

	int player, enemy;//判断玩家和敌人的行为
	game_atk = atk;
	game_hp = hp;
	int flag;//判断玩家是否操作
	yzzl = 0;//勇者之力技能指示
	effect_yzzl = 0;//勇者之力效果判断
	cure = 0;//治疗指示
	count_cure = 0;
	count_yzzl = 0;
	pkuangnu = 0;
	pzhuoshao = 0;
	count_shengguang = 0;
	end = 0;
	enemy_revive = 0;
	pzhongdu = 0;
	//被动技能判断↓先默认全部关闭，若携带则打开
	revive = 0;
	if (jinengnow.price == 1000)
	{
		revive = 1;
	}
	shengguang = 0;
	if (jinengnow.price == 300)
	{
		shengguang = 1;
	}
	fanjia = 0;
	if (jinengnow.price == 200)
	{
		fanjia = 1;
	}
	hanbin = 0;
	if (jinengnow.price == 500)
	{
		hanbin = 1;
	}
	xixue = 0;
	if (jinengnow.price == 100)
	{
		xixue = 1;
	}

	int bh;//敌人行为随机数

	switch (zhang)
	{
	case 0:
	{
		enemy_hp = 500;
		enemy_atk = 30;
		while (end != 1)
		{
			flag = 0;

			fightshow(zhang, 2,0);

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 65)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
					Sleep(100);
					for (int t = 0; t < 4; t++)
					{
						draw(550, 550, &jianguang[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					game_hp -= enemy_atk;
					if (fanjia == 1)
					{
						PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							end = 1;
							int dmoney = 200 * 3;
							money += dmoney;
							_itoa_s(dmoney, chardmoney, 10);
							ctot(print, chardmoney);
							outtextxy(560, 390, print);
							FlushBatchDraw();
							if (wc == 3)
							{
								wc += 1;
							}
							success(0, 2);
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
					}
					fightshow(zhang, 2,0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 2,0);
						}
					}
				}
				if (bh > 85 && bh <= 100)
				{
					pzhongdu = 1;
					mouse.lbutton = NULL;
					pzhongdu++;
					PlaySound(L"音效/中毒.wav", 0, SND_ASYNC);
					for (int i = 0; i < 3; i++)
					{
						draw(360, 400, &zhongdup[i]);
						FlushBatchDraw();
						Sleep(100);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("哥布林使用了投毒，下回合起每回合开始你将受到一定伤害"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 2,0);
				}
				if (bh > 80 && bh <= 85)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
					if (enemy_hp < 350)
					{
						enemy_hp = 350;
					}
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &cure_p[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					fightshow(zhang, 2,0);
				}
				if (bh > 65 && bh <= 80)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"音效/普通狂怒.wav", 0, SND_ASYNC);
					enemy_atk += 15;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("哥布林伤害提升了"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(500);
					fightshow(zhang, 2,0);
				}
			}
			Sleep(500);
			FlushBatchDraw();

			if (pzhongdu != 0)
			{
				for (int i = 0; i < pzhongdu; i++)
				{
					game_hp = game_hp - game_hp / 20;
					fightshow(zhang, 2,0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 2,0);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("触发重生"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(500);
							break;
						}
					}
				}
			}

			count_shengguang++;
			if (shengguang == 1 && count_shengguang % 2 == 0)
			{
				PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				f.lfHeight = 20;
				outtextxy(0, 500, _T("你的属性增强了"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(500);
				fightshow(zhang, 2,0);
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();
			mouse.lbutton = NULL;
			while (flag == 0)
			{
				Sleep(5);
				playerfight(&player, &flag, &zhang, 2,0);
			}
		}
		Sleep(250);
		break;
	}
	case 1:
	{
		enemy_hp = 2000;
		enemy_atk = 10;
		while (end != 1)
		{
			flag = 0;

			fightshow(zhang, 2, 0);

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh>=30&&bh <= 65)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"音效/普通狂怒.wav", 0, SND_ASYNC);
					enemy_atk += 20;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("亚米达拉伤害提升了"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(500);
					fightshow(zhang, 2, 0);
				}
				if (bh <= 50)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/sword1.wav", 0, SND_ASYNC);
					Sleep(100);
					for (int t = 0; t < 4; t++)
					{
						draw(550, 550, &jianguang[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					game_hp -= enemy_atk;
					if (fanjia == 1)
					{
						PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							end = 1;
							int dmoney = 200 * 3;
							money += dmoney;
							_itoa_s(dmoney, chardmoney, 10);
							ctot(print, chardmoney);
							outtextxy(560, 390, print);
							FlushBatchDraw();
							if (wc == 3)
							{
								wc += 1;
							}
							success(0, 2);
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
					}
					fightshow(zhang, 2, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 2, 0);
						}
					}
				}
				if (bh > 75 && bh <= 90)
				{
					mouse.lbutton = NULL;
					pzhongdu++;
					PlaySound(L"音效/中毒.wav", 0, SND_ASYNC);
					for (int i = 0; i < 3; i++)
					{
						draw(360, 400, &zhongdup[i]);
						FlushBatchDraw();
						Sleep(100);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("亚米达拉释放了毒气，下回合起每回合开始你将受到一定伤害"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 2, 0);
				}
				if (bh > 65 && bh <= 75)
				{
					mouse.lbutton = NULL;
					PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
					enemy_hp +=400;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &cure_p[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					f.lfHeight = 20;
					outtextxy(0, 500, _T("亚米达拉提升了血量"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(500);
					fightshow(zhang, 2, 0);
				}
				if (bh > 90 && bh <= 100)//混乱
				{
					phunluan = 1;
					count_hunluan+=3;
					mouse.lbutton = NULL;
					PlaySound(L"音效/混乱.wav", 0, SND_ASYNC);
					for (int i = 0; i < 3; i++)
					{
						draw(360, 400, &hunluan[i]);
						FlushBatchDraw();
						Sleep(100);
					}
					f.lfHeight = 20;
					outtextxy(0, 500, _T("亚米达拉严重影响了你的心智，你下次攻击有较大概率对自己造成等量伤害"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 2, 0);
				}
			}
			Sleep(500);
			FlushBatchDraw();

			if (pzhongdu != 0)
			{
				for (int i = 0; i < pzhongdu; i++)
				{
					game_hp = game_hp - game_hp / 20;
					fightshow(zhang, 2, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 2, 0);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("触发重生"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(500);
							break;
						}
					}
				}
			}

			count_shengguang++;
			if (shengguang == 1 && count_shengguang % 2 == 0)
			{
				PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				f.lfHeight = 20;
				outtextxy(0, 500, _T("你的属性增强了"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(500);
				fightshow(zhang, 2, 0);
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();
			mouse.lbutton = NULL;
			while (flag == 0)
			{
				Sleep(5);
				playerfight(&player, &flag, &zhang, 2, 0);
			}
		}
		Sleep(250);
		break;
	}
	case 2:
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	case 3:
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	}
}

void gameboss(int zhang)
{
	cleardevice();
	flushmessage(EM_MOUSE);

	LOGFONT f;//声明字体样式变量
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("方正姚体"));
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	int player, enemy;//判断玩家和敌人的行为
	game_atk = atk;
	game_hp = hp;
	int flag;//判断玩家是否操作
	yzzl = 0;//勇者之力技能指示
	effect_yzzl = 0;//勇者之力效果判断
	cure = 0;//治疗指示
	count_cure = 0;
	count_yzzl = 0;
	count_shengguang = 0;
	enemy_revive = 0;
	end = 0;
	phunluan = 0;
	pkuangnu = 0;
	pzhuoshao = 0;
	pzhongdu = 0;
	count_hunluan = 0;

	//被动技能判断↓先默认全部关闭，若携带则打开
	revive = 0;
	if (jinengnow.price == 1000)
	{
		revive = 1;
	}
	shengguang = 0;
	if (jinengnow.price == 300)
	{
		shengguang = 1;
	}
	fanjia = 0;
	if (jinengnow.price == 200)
	{
		fanjia = 1;
	}
	hanbin = 0;
	if (jinengnow.price == 500)
	{
		hanbin = 1;
	}
	xixue = 0;
	if (jinengnow.price == 100)
	{
		xixue = 1;
	}

	int bh;//敌人行为随机数

	switch (zhang) {//判断章节
	case 0:
	{
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open 音乐/魔兽世界.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

		int lhhx = 1;
		enemy_hp = 1500;
		enemy_atk = 30;
		putimage(0, 0, &game1_boss);
		outtextxy(300, 600, _T("但是，古尔丹，代价是什么呢？"));
		FlushBatchDraw();
		Sleep(3000);
		while (end != 1)
		{
			if (game_atk < 60)
			{
				game_atk += 3;
			}

			flag = 0;

			fightshow(zhang, 3,1);

			if (phunluan == 1)
			{
				draw(200, 650, &effect[4]);
				FlushBatchDraw();
			}

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;
			if (enemy_hp < 400 && lhhx == 1)
			{
				lhhx = 0;
				PlaySound(L"音效/灵魂虹吸.wav", 0, SND_ASYNC);
				enemy_hp += game_hp / 2+game_atk;
				game_hp = game_hp / 2;
				game_atk = game_atk / 2;
				for (int i = 0; i < 3; i++)
				{
					draw(360, 400, &hunluan[i]);
					FlushBatchDraw();
					Sleep(100);
				}
				for (int i = 1; i < 3; i++)
				{
					draw(500, 100, &hunluan[i]);
					FlushBatchDraw();
					Sleep(100);
				}
				for (int i = 2; i < 3; i++)
				{
					draw(700, 500, &hunluan[i]);
					FlushBatchDraw();
					Sleep(100);
				}
				for (int i = 1; i < 3; i++)
				{
					draw(100, 450, &hunluan[i]);
					FlushBatchDraw();
					Sleep(100);
				}
				for (int i = 0; i < 3; i++)
				{
					draw(200, 250, &hunluan[i]);
					FlushBatchDraw();
					Sleep(100);
				}
				for (int i = 0; i < 3; i++)
				{
					draw(1000, 225, &hunluan[i]);
					FlushBatchDraw();
					Sleep(100);
				}
				f.lfHeight = 20;
				outtextxy(0, 500, _T("古尔丹使用了灵魂虹吸，偷取了部分生命值，削弱了你的攻击力"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(700);
				fightshow(zhang, 3, 1);
			}
			else
			{
				if (effect_yzzl == 1)
				{
					PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
					effect_yzzl = 0;
				}
				else
				{
					if (bh <= 65)//古尔丹普通攻击
					{
						mouse.lbutton = NULL;
						PlaySound(L"音效/古尔丹普通攻击.wav", 0, SND_ASYNC);
						Sleep(100);
						for (int t = 0; t < 3; t++)
						{
							draw(800 - 150 * t, 100 + 250 * t, &fireball[t]);
							FlushBatchDraw();
							Sleep(150);
							fightshow(zhang, 3, 1);
						}
						PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
						for (int t = 0; t < 3; t++)
						{
							draw(500, 600, &boom[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						game_hp -= enemy_atk;
						if (fanjia == 1)
						{
							PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
							enemy_hp -= enemy_atk / 3;
							if (enemy_hp <= 0)
							{
								mciSendString(L"close 音乐/魔兽世界.mp3", 0, 0, 0);
								end = 1;
								int dmoney = 200 * 4;
								money += dmoney;
								_itoa_s(dmoney, chardmoney, 10);
								ctot(print, chardmoney);
								outtextxy(560, 390, print);
								FlushBatchDraw();
								if (wc == 4)
								{
									wc += 1;
								}
								success(0, 3);
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
								return;
							}
							f.lfHeight = 20;
							outtextxy(0, 500, _T("反甲触发"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(500);
						}
						fightshow(zhang, 3, 1);
						if (game_hp <= 0)
						{
							if (revive != 1)
							{
								mciSendString(L"close 音乐/魔兽世界.mp3", 0, 0, 0);
								defeat(zhang,3);
								end = 1;
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
								f.lfHeight = 20;
								outtextxy(0, 500, _T("重生触发"));
								f.lfHeight = 45;
								FlushBatchDraw();
								Sleep(700);
								game_hp = hp;
								pzhongdu = 0;
								phunluan = 0;
								revive = 0;
								fightshow(zhang, 3, 1);
							}
						}
					}
					if (bh > 65 && bh <= 70)//古尔丹回血
					{
						mouse.lbutton = NULL;
						PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
						if (enemy_hp < 1500)
						{
							enemy_hp += 500;//恢复500血
						}
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &cure_p[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("古尔丹恢复了部分血量"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(700);
						fightshow(zhang, 3, 1);
					}
					if (bh > 70 && bh <= 80)//混乱
					{
						phunluan = 1;
						count_hunluan++;
						mouse.lbutton = NULL;
						PlaySound(L"音效/混乱.wav", 0, SND_ASYNC);
						for (int i = 0; i < 3; i++)
						{
							draw(360, 400, &hunluan[i]);
							FlushBatchDraw();
							Sleep(100);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("古尔丹使用了混乱,你下次攻击有概率对自己造成等量伤害"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(700);
						fightshow(zhang, 3, 1);
					}
					if (bh > 80 && bh <= 90)
					{
						mouse.lbutton = NULL;
						pzhongdu++;
						PlaySound(L"音效/中毒.wav", 0, SND_ASYNC);
						for (int i = 0; i < 3; i++)
						{
							draw(360, 400, &zhongdup[i]);
							FlushBatchDraw();
							Sleep(100);
						}
						f.lfHeight = 20;
						outtextxy(0, 500, _T("古尔丹释放了毒气,下回合开始每回合你将受到一定伤害"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);
						fightshow(zhang, 3, 1);
					}
					if (bh > 90 && bh <= 100)
					{
						pkuangnu = 1;
						mouse.lbutton = NULL;
						PlaySound(L"音效/普通狂怒.wav", 0, SND_ASYNC);
						enemy_atk += 15;
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &kuangnu[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("古尔丹提升了攻击力"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(700);
						fightshow(zhang, 3, 1);
					}
				}
			}
			Sleep(500);
			FlushBatchDraw();

			if (pzhongdu != 0)
			{
				for (int i = 0; i < pzhongdu; i++)
				{
					game_hp = game_hp - game_hp / 20;
					fightshow(zhang, 3, 1);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,3);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang,3, 1);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("触发重生"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(500);
							break;
						}
					}
				}
			}

			count_shengguang++;
			if (shengguang == 1 && count_shengguang % 2 == 0)
			{
				PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				f.lfHeight = 20;
				outtextxy(0, 500, _T("你的属性得到增强"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(800);
				fightshow(zhang,3,1);
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();
			mouse.lbutton = NULL;
			while (flag == 0)
			{
				Sleep(5);
				playerfight(&player, &flag, &zhang, 3,1);
			}
		}
		Sleep(250);
		break;
	}
	case 1:
	{
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open 音乐/尤姆.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);

		int strongatk = 0;
		xinwang = 0;
		if (weaponnow.atk == 22)
		{
			game_atk = 1000+atk;
			enemy_atk = 5;
		}
		enemy_hp = 9999;
		enemy_atk = 100;
		putimage(0, 0, &game2_boss);
		if (weaponnow.atk != 22)
		{
			outtextxy(300, 600, _T("愿薪王与太阳同在"));
		}
		else
		{
			outtextxy(300, 600, _T("你来履行诺言了"));
		}
		FlushBatchDraw();
		Sleep(3000);

		while (end != 1)
		{
			if (game_atk < 100)
			{
				game_atk += 3;
			}

			flag = 0;

			fightshow(zhang, 3, 1);

			if (count_cure < 5)
			{
				count_cure++;
			}
			if (count_cure == 5)
			{
				cure = 0;
			}
			if (count_yzzl < 5)
			{
				count_yzzl++;
			}
			if (count_yzzl == 5)
			{
				yzzl = 0;
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();

			srand((unsigned int)time(NULL));//敌人行为
			bh = rand() % 100 + 1;

			if (strongatk == 1)//强力一击
			{
				if (weaponnow.atk == 22&&effect_yzzl!=1)
				{
					PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
					PlaySound(L"音效/暴风大剑.wav", 0, SND_ASYNC);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("强力一击被抵消了,因为风暴之力"));
					Sleep(3000);
					f.lfHeight = 45;
					FlushBatchDraw();
					strongatk = 0;
				}
				if (weaponnow.atk == 22 && effect_yzzl == 1)
				{
					PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
					PlaySound(L"音效/暴风大剑.wav", 0, SND_ASYNC);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("强力一击被抵消了,因为风暴之力"));
					f.lfHeight = 45;
					FlushBatchDraw();
					strongatk = 0;
				}
				if (effect_yzzl == 1 && weaponnow.atk != 22)
				{
					PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("强力一击被抵消了,因为勇者之力"));
					f.lfHeight = 45;
					FlushBatchDraw();
					strongatk = 0;
					effect_yzzl = 0;
				}
				Sleep(500);

				if (strongatk == 1)
				{
					strongatk = 0;

					PlaySound(L"音效/灼烧.wav", 0, SND_ASYNC);
					PlaySound(L"音效/灵魂虹吸.wav", 0, SND_ASYNC);
					PlaySound(L"音效/灼烧.wav", 0, SND_ASYNC);
					PlaySound(L"音效/灼烧.wav", 0, SND_ASYNC);

					pzhuoshao = 1;//灼烧效果

					mouse.lbutton = NULL;
					PlaySound(L"音效/巨人王蓄力.wav", 0, SND_ASYNC);
					Sleep(3000);
					PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
					for (int t = 0; t < 3; t++)
					{
						draw(500, 600, &boom[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					for (int t = 0; t < 3; t++)
					{
						draw(400, 610, &boom[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					for (int t = 0; t < 3; t++)
					{
						draw(530, 570, &boom[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					for (int t = 0; t < 3; t++)
					{
						draw(520, 540, &boom[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					for (int t = 0; t < 3; t++)
					{
						draw(570, 590, &boom[t]);
						FlushBatchDraw();
						Sleep(50);
					}
					game_hp -= enemy_atk * 5;

					f.lfHeight = 20;
					outtextxy(0, 500, _T("巨人王尤姆释放了强力一击"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 3, 1);

					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close BGM", 0, 0, 0);
							defeat(zhang, 3);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 3, 1);
						}
					}
				}
			}

			if (enemy_hp < 4000 && xinwang == 0)
			{
				xinwang = 1;//进入二阶段
				strongatk = 1;
				enemy_atk += 200;

				mciSendString(L"close BGM", 0, 0, 0);
				mciSendString(L"open 音乐/巨人王尤姆.mp3 alias BGM", 0, 0, 0);
				mciSendString(L"play BGM repeat", 0, 0, 0);
				mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);

				enemy_hp = 4000;

				pkuangnu = 1;
				mouse.lbutton = NULL;
				enemy_atk += 100;
				for (int i = 0; i < 3; i++)
				{
					draw(360, 100, &kuangnuplus[i]);
					FlushBatchDraw();
					Sleep(70);
				}
				Sleep(200);

				f.lfHeight = 20;
				outtextxy(0, 500, _T("尤姆进入了薪王状态，属性增强并将发起一次强力攻击"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(1500);
				fightshow(zhang, 3, 1);

			}
			else
			{
				if (effect_yzzl == 1)
				{
					PlaySound(L"音效/圣盾.wav", 0, SND_ASYNC);
					Sleep(500);
					fightshow(zhang, 3, 1);
					effect_yzzl = 0;
				}
				else
				{
					if (bh <= 75)
					{
						mouse.lbutton = NULL;
						PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
						for (int t = 0; t < 3; t++)
						{
							draw(500, 600, &boom[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						game_hp -= enemy_atk;
						if (fanjia == 1)
						{
							PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
							enemy_hp -= enemy_atk / 5;
							if (enemy_hp <= 0)
							{
								mciSendString(L"close BGM", 0, 0, 0);
								end = 1;
								int dmoney = 200 * 4;
								money += dmoney;
								_itoa_s(dmoney, chardmoney, 10);
								ctot(print, chardmoney);
								outtextxy(560, 390, print);
								FlushBatchDraw();
								if (wc == 8)
								{
									wc += 1;
								}
								success(1, 3);
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
								return;
							}
							f.lfHeight = 20;
							outtextxy(0, 500, _T("反甲触发"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(500);
						}
						fightshow(zhang, 3, 1);
						if (game_hp <= 0)
						{
							if (revive != 1)
							{
								mciSendString(L"close BGM", 0, 0, 0);
								defeat(zhang,3);
								end = 1;
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
								f.lfHeight = 20;
								outtextxy(0, 500, _T("重生触发"));
								f.lfHeight = 45;
								FlushBatchDraw();
								Sleep(700);
								game_hp = hp;
								pzhongdu = 0;
								phunluan = 0;
								revive = 0;
								fightshow(zhang, 3, 1);
							}
						}
					}
					if (bh > 75 && bh <= 80)//巨人王回血
					{
						mouse.lbutton = NULL;
						PlaySound(L"音效/治疗.wav", 0, SND_ASYNC);
						if (enemy_hp < 9999)
						{
							enemy_hp += 1000;//恢复1000血
						}
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &cure_p[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("巨人王恢复了部分血量"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);
						fightshow(zhang, 3, 1);
					}
					if (bh > 80 && bh <= 100)
					{
						mouse.lbutton = NULL;
						PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
						Sleep(300);
						PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
						Sleep(300);
						PlaySound(L"音效/爆炸.wav", 0, SND_ASYNC);
						for (int t = 0; t < 3; t++)
						{
							draw(500, 600, &boom[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						for (int t = 0; t < 3; t++)
						{
							draw(400, 610, &boom[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						for (int t = 0; t < 3; t++)
						{
							draw(530, 570, &boom[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						game_hp -= enemy_atk * 3;
						if (fanjia == 1)
						{
							PlaySound(L"音效/命中.wav", 0, SND_ASYNC);
							enemy_hp -= enemy_atk/2;
							if (enemy_hp <= 0)
							{
								mciSendString(L"close BGM", 0, 0, 0);
								end = 1;
								int dmoney = 200 * 2;
								money += dmoney;
								_itoa_s(dmoney, chardmoney, 10);
								ctot(print, chardmoney);
								outtextxy(560, 390, print);
								FlushBatchDraw();
								if (wc == 2)
								{
									wc += 1;
								}
								success(1, 3);
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
								return;
							}
						}
						f.lfHeight = 20;
						outtextxy(0, 500, _T("巨人王使用了三段连击"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);
						fightshow(zhang, 3, 1);
						if (game_hp <= 0)
						{
							if (revive != 1)
							{
								mciSendString(L"close BGM", 0, 0, 0);
								defeat(zhang,3);
								end = 1;
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
								game_hp = hp;
								revive = 0;
								fightshow(zhang, 3, 1);
							}
						}
					}
				}
			}
			Sleep(500);
			FlushBatchDraw();

			if (pzhuoshao != 0)
			{
				for (int i = 0; i < pzhuoshao; i++)
				{
					game_hp -=20;
					fightshow(zhang, 3, 1);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close 音乐/魂斗罗.mp3", 0, 0, 0);
							defeat(zhang,3);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"音效/重生.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 3, 1);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("触发重生"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(500);
							break;
						}
					}
				}
			}

			count_shengguang++;
			if (shengguang == 1 && count_shengguang % 2 == 0)
			{
				PlaySound(L"音效/圣光.wav", 0, SND_ASYNC);
				game_hp += 20;
				game_atk += 20;
				f.lfHeight = 20;
				outtextxy(0, 500,_T("属性增强"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(800);
				fightshow(zhang, 3, 1);
			}

			if (count_cure != 5 && cure == 1)
			{
				_itoa_s(5 - count_cure, cure_count, 10);
				ctot(print, cure_count);
				outtextxy(1005, 635, print);
			}
			if (count_yzzl != 5 && yzzl == 1)
			{
				_itoa_s(5 - count_yzzl, yzzl_count, 10);
				ctot(print, yzzl_count);
				outtextxy(1005, 530, print);
			}
			FlushBatchDraw();
			mouse.lbutton = NULL;
			while (flag == 0)
			{
				Sleep(5);
				playerfight(&player, &flag, &zhang, 3, 1);
			}
		}
		Sleep(250);
		break;
	}
	case 2:
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	case 3:
	{
		while (1)
		{
			putimage(0, 0, &background[zhang][0]);
			draw(0, 0, &zhandoukuang);
			draw(0, 0, &xueliang);
			draw(500, 100, &actor[zhang][0]);
			draw(500, 500, &mactorshow);
			FlushBatchDraw();
			Sleep(200);
		}
		break;
	}
	}
}

void zhangjiebianhuan(int *change,int game_levels,int zhangjie)
{
	LOGFONT f;//声明字体样式变量
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("方正姚体"));
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
	f.lfHeight = 60;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	int nz = 0;
	int c = *change;
	int times = 0;
	int* pn = &nz;

	BeginBatchDraw();
	putimage(0, 0, &mb[c]);//进入存档自动到第一章界面
	draw(1100, 0, &barp);
	if (c < zhangjie-1)
	{
		draw(1100, 300, &directR);//画→箭头
	}
	if (c > 0&&c<=zhangjie-1)
	{
		draw(0, 300, &directL);//画←箭头
	}
	if (c == zhangjie-1)
	{
		while (nz < game_levels&&nz!=3)//画关卡
		{
			game_level(pn);
			times++;
		}
		if (game_levels==4)
		{
			draw((nz+1) * 200, 350 + 150 * pow(-1, nz), &level1BOSS);
		}
	}
	else
	{
		while (nz < 3)//画关卡
		{
			game_level(pn);
		}
		if (game_levels == 4||c<zhangjie)
		{
			draw((nz + 1) * 200, 350 + 150 * pow(-1, nz), &level1BOSS);
		}
	}
	while (peekmessage(&mouse, EM_MOUSE))
	{
		switch (mouse.message)
		{
		case WM_LBUTTONDOWN:
			if (mouse.lbutton && (mouse.x > 1100 && mouse.x < 1189) && (mouse.y > 0 && mouse.y < 89))//进入酒馆
			{
				jiuguan(&c);
				settextcolor(WHITE);
				settextstyle(&f);
			}
			if (mouse.lbutton && c < zhangjie - 1 && c >= 0 && (mouse.x > 1100 && mouse.x < 1200) && (mouse.y > 300 && mouse.y < 400))
			{
				mouse.lbutton = 0;
				++c;
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/黑暗之魂3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/暗黑破坏神3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/最终.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
				flushmessage(EM_MOUSE);
			}
			if (mouse.lbutton && c > 0 && c <= zhangjie - 1 && (mouse.x > 0 && mouse.x < 100) && (mouse.y > 300 && mouse.y < 400))
			{
				mouse.lbutton = 0;
				--c;
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/黑暗之魂3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/暗黑破坏神3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/最终.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
				flushmessage(EM_MOUSE);
			}
			*change = c;

			if ((c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 1)) && mouse.lbutton && (mouse.x > 200 && mouse.x < 264) && (mouse.y > 500 && mouse.y < 564))//进入第一关
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				game1(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/黑暗之魂3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/暗黑破坏神3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/最终.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
			}

			if ((c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 2)) && mouse.lbutton && (mouse.x > 400 && mouse.x < 464) && (mouse.y > 200 && mouse.y < 264))//进入第二关
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				game2(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/黑暗之魂3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/暗黑破坏神3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/最终.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
			}

			if ((c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 3)) && mouse.lbutton && (mouse.x > 600 && mouse.x < 664) && (mouse.y > 500 && mouse.y < 564))//进入第三关
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				game3(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/黑暗之魂3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/暗黑破坏神3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/最终.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
			}

			if ((game_levels==4||c<zhangjie)&&(c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 4)) && mouse.lbutton && (mouse.x > 800 && mouse.x < 864) && (mouse.y > 200 && mouse.y < 264))//进入BOSS关
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				gameboss(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/黑暗之魂3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/暗黑破坏神3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open 音乐/最终.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
				break;
			}
		}
	}
	EndBatchDraw();
	Sleep(100);
}

void game(int re)
{
	BeginBatchDraw();
	FILE* file = NULL;//定义文件
	int error;//报错信息
	int change = 0;//换章节用变量
	int* pc = &change;//换章节用指针
	loadimage(&barp, L"图标/酒馆图标.png");
	loadimage(&level1, L"图标/第一章图标.png");
	loadimage(&level1BOSS, L"图标/第一章图标BOSS.png");
	loadimage(&directR, L"背景/右箭头.png",100,100);
	loadimage(&directL, L"背景/左箭头.png",100,100);
	loadimage(&mb[0], L"背景/幽暗密林.jpg", 1200, 700);
	loadimage(&mb[1], L"背景/罪业之都.jpg", 1200, 700);
	loadimage(&mb[2], L"背景/火之炼狱.jpg", 1200, 700);
	loadimage(&mb[3], L"背景/苍穹之间.jpg", 1200, 700);
	loadimage(&gback, L"背景/游戏里返回.png");
	loadimage(&tishi, L"背景/提示.jpg");
	if (re == 1)
	{
		error = fopen_s(&file,"存档/存档一.txt", "r");
	}
	if (re == 2)
	{
		error = fopen_s(&file,"存档/存档二.txt", "r");
	}
	if (re == 3)
	{
		error = fopen_s(&file,"存档/存档三.txt", "r");
	}
	if (error != 0)
	{
		exit(0);
	}
	fscanf_s(file, "%d\n", &zhangjie);//读取游戏章节
	fscanf_s(file, "%d\n", &game_levels);//读取游戏章节内的关卡状况
	fscanf_s(file, "%d %d\n", &hp, &atk);//读取生命值和攻击力
	fscanf_s(file, "%d\n", &wc);//读取已获得武器
	fscanf_s(file, "%d\n", &wn);//现在装备的武器编号
	fscanf_s(file, "%d\n", &wjineng);//现在装备的技能编号
	fscanf_s(file, "%d\n", &money);//现在装备的武器编号
	fscanf_s(file, "%d\n", &newopen);//是否第一次玩
	fclose(file);
	Sleep(200);

	for (int i = 0; i < wc; i++)
	{
		myweapon[i] = w[i];
	}
	weaponnow = myweapon[wn];
	jinengnow = jinengall[wjineng];

	if (newopen == 1)
	{
		cleardevice();
		newopen = 0;
		draw(0, 0, &newgame);
		FlushBatchDraw();
		save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//存档
		Sleep(8000);
	}

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open 音乐/格兰之森.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
	while (1)
	{
		zhangjiebianhuan(pc, game_levels,zhangjie);
	}
	FlushBatchDraw();
	EndBatchDraw();
}

int main()
{
	initgame();
	peekmessage(&mouse, EX_MOUSE);//鼠标获取信息函数
	setbkcolor(WHITE);

	HCURSOR hcur = LoadCursorFromFile(TEXT("图标/鼠标.cur"));//设置鼠标光标样式
	HWND hwnd = GetHWnd();
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur);

	IMAGE tips;//启动提示
	loadimage(&tips, L"背景/Tips.png",1200,700);
	putimage(0, 0, &tips);
	Sleep(3000);

	int duqu=0;//选择存档用数据
	re = menu();
	while (1)
	{
		game(re);
	}
	system("pause");
	return 0;
}