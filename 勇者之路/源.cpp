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
ExMessage mouse;//�������
IMAGE actor[4][5], mactor[4], mactorR[4], mactorL[4], mactorB[4],
tishi, bk, bk1, bk2, bk3, cundang, back, cundang1, mb[4], barp, directR, directL,
level1, level1BOSS, gback, mbar, bar1, food1, food2, food3, mweapon, gw[16],wp[16],effect[8],
background[4][2],zhandoukuang,xueliang,mactorshow,jinengstore,jinengp[5],jinbi,jianguang[4],cure_p[3],xuli[3],boom[3],
game1_1,game1_boss,win,fail,kuangnu[3],zhongdup[3],fireball[3],hunluan[3],kuangnuplus[3],game2_boss,newgame;

typedef struct
{
	IMAGE* p;  //ͼƬ
	char name[20]; //��������
	int atk;  //������
	TCHAR story[30];//����
}weapon;//����
weapon w[16];//������Ϸ�����������Ľṹ��
weapon myweapon[16];//������Ϸ�浵ӵ�е������Ľṹ��
weapon weaponnow;//װ��������

typedef struct
{
	IMAGE* p;  //ͼƬ
	char name[20]; //��������
	TCHAR story[30];//����
	int price;//�۸�
}jineng;//����
jineng jinengnow;//װ���ļ���
jineng jinengall[5];//�������м��ܵĽṹ��
jineng myjineng[5];//�浵ӵ�еļ���

int x, y;//��������
int dir=1;//���﷽��

int hp = 100;//����Ѫ��
int game_hp;//�ؿ���Ѫ��
char php[10];//��ӡѪ�����ַ���
int atk = 15;//���﹥����
int game_atk;//�ؿ��ڹ�����
int money = 0;//��Ǯ����
char pmoney[10];//��ӡ��Ǯ����
char chardmoney[10];
char cure_count[10];
char yzzl_count[10];

int yzzl = 1;//����֮������ָʾ
int effect_yzzl = 0;
int cure = 1;//����ָʾ
int count_cure = 0;
int count_yzzl = 0;
int revive = 0;//�����õ��ĸ�����ж�
int shengguang = 0;//�����õ���ʥ���ж�
int count_shengguang = 0;//ʥ����غ�����
int xixue = 0;//�����õ�����Ѫ�ж�
int fanjia = 0;//�����õ��ķ����ж�
int hanbin = 0;//�����õ��ĺ����ж�

int enemy_hp;//����Ѫ��
int enemy_atk;//���˹�����
int enemy_revive;//����ӵ�еĸ����
int pkuangnu = 0;//����ӵ�еĿ�ŭ����
int pzhongdu = 0;//����ӵ�е��ж�����
int phunluan = 0;//����ӵ�еĻ��Ҽ���
int pzhuoshao = 0;//����ӵ�е����ռ���
int xinwang = 0;
int count_hunluan = 0;

int re;//�浵���
int game_levels, zhangjie;//�浵��Ҫ��һϵ�б���
int wc, wn,wjineng,newopen;

char patk[10];//��ӡ���������ַ���
char pprice[10]; //��ӡ�۸����ַ���
TCHAR print[30];//��ӡ�ı����ַ���

int end;//�ж���Ϸ����

void save(int re,int zhangjie, int game_level, int atk, int hp, int weaponnum,int weaponnow,int jinengnow,int money,int newopen)
{
	FILE* file = NULL;//�����ļ�
	int error;
	if (re == 1)
	{
		error = fopen_s(&file, "�浵/�浵һ.txt", "w");
	}
	if (re == 2)
	{
		error = fopen_s(&file, "�浵/�浵��.txt", "w");
	}
	if (re == 3)
	{
		error = fopen_s(&file, "�浵/�浵��.txt", "w");
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
int ccount = 0;//���ﶯ������
//���� - char�ַ���ת��ΪTCHAR�ַ�����
void ctot(TCHAR* Tstr, char* str) {

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, str, -1, Tstr, 20);
#else
	strcpy(Tstr, str);
#endif
}
void draw(int x, int y, IMAGE* image, int pic_x = 0, int pic_y = 0, double AA = 1)
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();   // GetImageBuffer() ���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬 EasyX �Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(image);  // ��ȡ picture ���Դ�ָ��
	int imageWidth = image->getwidth();  // ��ȡͼƬ���
	int imageHeight = image->getheight(); // ��ȡͼƬ���
	int dstX = 0;       // �� ��ͼ���� �Դ������صĽǱ�
	int srcX = 0;       // �� image �Դ������صĽǱ�
	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < imageHeight; iy++)
	{
		for (int ix = 0; ix < imageWidth; ix++)
		{
			// ��ֹԽ��
			if (ix + pic_x >= 0 && ix + pic_x < imageWidth && iy + pic_y >= 0 && iy + pic_y < imageHeight &&
				ix + x >= 0 && ix + x < LENGTH && iy + y >= 0 && iy + y < WIDTH)
			{
				// ��ȡ���ؽǱ�
				int sa = 0, sr = 0, sg = 0, sb = 0;
				int srcX = (ix + pic_x) + (iy + pic_y) * imageWidth;
				dstX = (ix + x) + (iy + y) * LENGTH;

				sa = ((src[srcX] & 0xff000000) >> 24) * AA;   // 0xAArrggbb; AA ��͸����
				sr = ((src[srcX] & 0xff0000) >> 16);    // ��ȡ RGB ��� R
				sg = ((src[srcX] & 0xff00) >> 8);     // G
				sb = src[srcX] & 0xff;        // B

				// ���ö�Ӧ�Ļ�ͼ����������Ϣ
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

void initgame()
{
	initgraph(1200, 700);
	loadimage(&mactor[0], L"��ɫ/����/����(1).png", 60, 80);
	loadimage(&mactor[1], L"��ɫ/����/����(2).png", 60, 80);
	loadimage(&mactor[2], L"��ɫ/����/����(3).png", 60, 80);
	loadimage(&mactor[3], L"��ɫ/����/����(4).png", 60, 80);
	loadimage(&mactorR[0], L"��ɫ/����/����R(1).png", 60, 80);
	loadimage(&mactorR[1], L"��ɫ/����/����R(2).png", 60, 80);
	loadimage(&mactorR[2], L"��ɫ/����/����R(3).png", 60, 80);
	loadimage(&mactorR[3], L"��ɫ/����/����R(4).png", 60, 80);
	loadimage(&mactorL[0], L"��ɫ/����/����L(1).png", 60, 80);
	loadimage(&mactorL[1], L"��ɫ/����/����L(2).png", 60, 80);
	loadimage(&mactorL[2], L"��ɫ/����/����L(3).png", 60, 80);
	loadimage(&mactorL[3], L"��ɫ/����/����L(4).png", 60, 80);
	loadimage(&mactorB[0], L"��ɫ/����/����B(1).png", 60, 80);
	loadimage(&mactorB[1], L"��ɫ/����/����B(2).png", 60, 80);
	loadimage(&mactorB[2], L"��ɫ/����/����B(3).png", 60, 80);
	loadimage(&mactorB[3], L"��ɫ/����/����B(4).png", 60, 80);
	loadimage(&mactorshow, L"��ɫ/����/����B(1).png", 150,200);
	loadimage(&actor[0][0], L"��ɫ/�İ�����/��������.png");
	loadimage(&actor[0][1], L"��ɫ/�İ�����/ţͷ��.png",350,300);
	loadimage(&actor[0][2], L"��ɫ/�İ�����/�粼��.png",450,300);
	loadimage(&actor[0][3], L"��ɫ/�İ�����/�Ŷ���.png",400,540);
	loadimage(&actor[1][0], L"��ɫ/��ҵ֮��/����.png", 600, 450);
	loadimage(&actor[1][1], L"��ɫ/��ҵ֮��/������ʿ.png", 300, 340);
	loadimage(&actor[1][2], L"��ɫ/��ҵ֮��/���״���.png", 1200, 675);
	loadimage(&actor[1][3], L"��ɫ/��ҵ֮��/��������ķ1.png", 1000, 554);
	loadimage(&actor[1][4], L"��ɫ/��ҵ֮��/��������ķ2.png", 1000, 554);
	loadimage(&wp[0], L"����/��ש.png",44,47);
	loadimage(&wp[1], L"����/�˹�.png", 44, 47);
	loadimage(&wp[2], L"����/����.png", 44, 47);
	loadimage(&wp[3], L"����/�𹿰�.png", 44, 47);
	loadimage(&wp[4], L"����/��ʯ��.png", 44, 47);
	loadimage(&wp[5], L"����/�籩������.png", 44, 47);
	loadimage(&wp[6], L"����/�ڰ���.png", 44, 47);
	loadimage(&wp[7], L"����/����.png", 44, 47);
	loadimage(&wp[8], L"����/����֮��.png", 44, 47);
	loadimage(&wp[9], L"����/�޾�֮��.png", 44, 47);
	loadimage(&wp[10], L"����/˪֮����.png", 44, 47);
	loadimage(&wp[11], L"����/����֮��.png", 44, 47);
	loadimage(&wp[12], L"����/֥ʿ.png", 44, 47);
	loadimage(&wp[13], L"����/�⽣.png", 44, 47);
	loadimage(&wp[14], L"����/�ƻ���.png", 44, 47);
	loadimage(&wp[15], L"����/�����.png", 44, 47);
	w[0] = { &wp[0],("��ש"),5,_T("���������ŵ��ǰ�ש��������") };
	w[1] = { &wp[1],("�˹�"),10,_T("����ѧʥ��") };
	w[2] = { &wp[2],("����"),15,_T("����������һ��ľ��") };
	w[3] = { &wp[3],("�𹿰�"),25,_T("�ٲ��µ�һ����Ѩ�����ҵ���") };
	w[4] = { &wp[4],("��ʯ��"),20,_T("������ʯ��һ��ľ��") };
	w[5] = { &wp[5],("�籩������"),22,_T("ֻ�з籩���ܻ���������FNNDP--Mr.Quin") };
	w[6] = { &wp[6],("�ڰ���"),30,_T("�ڰ���22������") };
	w[7] = { &wp[7],("����"),5,_T("һ����ͨ�����򣬵ȵȣ���Ҫ�ز����") };
	w[8] = { &wp[8],("����֮��"),40,_T("��ŷ�Ķ�����ô���⣬������ͽ𹿰�˭�أ�") };
	w[9] = { &wp[9],("�޾�֮��"),45,_T("�𼱣����Ұ��޾���������") };
	w[10] = { &wp[10],("˪֮����"),50,_T("���׽�����ĺ�ѡ��") };
	w[11] = { &wp[11],("����֮��"),60,_T("����˹��!����ƥ˹ɽ������η���ң�") };
	w[12] = { &wp[12],("֥ʿ"),5,_T("֥ʿ��������,��֪������������˵ѩ�������Ҳ�˵") };
	w[13] = { &wp[13],("�⽣"),70,_T("ԭ������ͬ�ڣ����߿Ƽ����������") };
	w[14] = { &wp[14],("�ƻ���"),75,_T("���͵�ʲôʱ��ѵٷ����ܸ���") };
	w[15] = { &wp[15],("�����"),9999,_T("�����ѽ�Ϩ��Ȼλ������Ӱ") };
	loadimage(&effect[0], L"��Ч/�ж�.png", 44, 47);
	loadimage(&effect[1], L"��Ч/����.png", 44, 47);
	loadimage(&effect[2], L"��Ч/��Ĭ.png", 44, 47);
	loadimage(&effect[3], L"��Ч/����.png", 44, 47);
	loadimage(&effect[4], L"��Ч/����.png", 44, 47);
	loadimage(&effect[5], L"��Ч/����.png", 44, 47);
	loadimage(&effect[6], L"��Ч/�޷��ж�.png", 44, 47);
	loadimage(&effect[7], L"��Ч/����.png", 44, 47);
	loadimage(&jinengp[0], L"��Ч/��Ѫ.png", 44, 47);
	loadimage(&jinengp[1], L"��Ч/����֮��.png", 44, 47);
	loadimage(&jinengp[2], L"��Ч/ʥ��.png", 44, 47);
	loadimage(&jinengp[3], L"��Ч/����֮��.png", 44, 47);
	loadimage(&jinengp[4], L"��Ч/����.png", 44, 47);
	loadimage(&jianguang[0], L"��Ч/����/����1.png", 140, 180);
	loadimage(&jianguang[1], L"��Ч/����/����2.png", 140, 180);
	loadimage(&jianguang[2], L"��Ч/����/����3.png", 140, 180);
	loadimage(&jianguang[3], L"��Ч/����/����4.png", 140, 180);
	loadimage(&cure_p[0], L"��Ч/�ָ�/�ָ�1.png",128,128);
	loadimage(&cure_p[1], L"��Ч/�ָ�/�ָ�2.png",128,128);
	loadimage(&cure_p[2], L"��Ч/�ָ�/�ָ�3.png",128,128);
	loadimage(&kuangnu[0], L"��Ч/��ͨ��ŭ/��ŭ1.png", 128, 128);
	loadimage(&kuangnu[1], L"��Ч/��ͨ��ŭ/��ŭ2.png", 128, 128);
	loadimage(&kuangnu[2], L"��Ч/��ͨ��ŭ/��ŭ3.png", 128, 128);
	loadimage(&kuangnuplus[0], L"��Ч/��������ŭ/��������ŭ1.png", 128, 128);
	loadimage(&kuangnuplus[1], L"��Ч/��������ŭ/��������ŭ2.png", 128, 128);
	loadimage(&kuangnuplus[2], L"��Ч/��������ŭ/��������ŭ3.png", 128, 128);
	loadimage(&xuli[0], L"��Ч/����/����1.png", 128, 128);
	loadimage(&xuli[1], L"��Ч/����/����2.png", 128, 128);
	loadimage(&xuli[2], L"��Ч/����/����3.png", 128, 128);
	loadimage(&boom[0], L"��Ч/��ը/��ը1.png", 128, 128);
	loadimage(&boom[1], L"��Ч/��ը/��ը2.png", 128, 128);
	loadimage(&boom[2], L"��Ч/��ը/��ը3.png", 128, 128);
	loadimage(&zhongdup[0], L"��Ч/����/����1.png", 128, 128);
	loadimage(&zhongdup[1], L"��Ч/����/����2.png", 128, 128);
	loadimage(&zhongdup[2], L"��Ч/����/����3.png", 128, 128);
	loadimage(&fireball[0], L"��Ч/����/����1.png", 128, 128);
	loadimage(&fireball[1], L"��Ч/����/����2.png", 128, 128);
	loadimage(&fireball[2], L"��Ч/����/����3.png", 128, 128);
	loadimage(&hunluan[0], L"��Ч/����/����1.png", 128, 128);
	loadimage(&hunluan[1], L"��Ч/����/����2.png", 128, 128);
	loadimage(&hunluan[2], L"��Ч/����/����3.png", 128, 128);
	jinengall[0] = { &jinengp[0],("��Ѫ"),_T("���㹥������ʱ����һ���̶�����") ,100};
	jinengall[1] = { &jinengp[1],("����֮��"),_T("�ڵ��˹�����ʱ���ܵ�һ���̶��˺�") ,200};
	jinengall[2] = { &jinengp[2],("ʥ��"),_T("ÿ2�غϻ��һ�ιؿ���������ǿ") ,300};
	jinengall[3] = { &jinengp[3],("����֮��"),_T("ӵ�к����������������ض��½�����"),500 };
	jinengall[4] = { &jinengp[4],("����"),_T("һ����սֻ��ʩ��һ�Σ���ɫ�����󸴻�"),1000 };
	loadimage(&jinbi, L"����/���ҳ��.png");
	loadimage(&zhandoukuang, L"����/���ܿ�.png");
	loadimage(&xueliang, L"����/Ѫ��.png");
	loadimage(&win, L"����/ʤ��.png");
	loadimage(&fail, L"����/ʧ��.png");
	loadimage(&background[0][0], L"����/��һ��/С��.jpg", 1200, 700);
	loadimage(&background[0][1], L"����/��һ��/BOSS.jpg", 1200, 700);
	loadimage(&background[1][0], L"����/�ڶ���/С��.jpg", 1200, 700);
	loadimage(&background[1][1], L"����/�ڶ���/BOSS.jpg", 1200, 700);
	loadimage(&game1_1, L"����/��һ��/��һ�ضԻ�����.jpg", 1200, 700);
	loadimage(&game1_boss, L"����/��һ��/BOSS�Ի�����.jpg");
	loadimage(&game2_boss, L"����/�ڶ���/BOSS�Ի�����.jpg");
	loadimage(&newgame, L"����/�������.png",1200,700);
}

void success(int zhang,int jie)
{
	mciSendString(L"close BGM", 0, 0, 0);
	PlaySound(L"����/ʤ��.wav", 0, SND_ASYNC);
	draw(0,0, &win);
	if (zhang == 1 && jie == 3 && weaponnow.atk!=22)
	{
		outtextxy(100, 600, _T("ֻ�з籩���ܻ���������FNNDP"));
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
	PlaySound(L"����/ʧ��.wav", 0, SND_ASYNC);
	draw(0, 0, &fail);
	if (zhang == 1 && jie == 3&&weaponnow.atk!=22)
	{
		outtextxy(300, 600, _T(" ֻ�з籩���ܻ�������"));
	}
	FlushBatchDraw();
	Sleep(5000);
	return;
}

void fightshow(int zhang,int jie,int changjing)
{
	putimage(0, 0, &background[zhang][changjing]);//����
	if (zhang == 0 && jie == 3)
	{
		draw(400, 0, &actor[zhang][jie]);//�Ŷ���
	}
	else
	{
		if (zhang == 1 && jie == 2)
		{
			draw(0, -180, &actor[zhang][jie]);//���״���
		}
		else
		{
			if (zhang == 1 && jie == 3 && xinwang == 0)
			{
				draw(70, -20, &actor[zhang][jie]);//��ķ1
			}
			else
			{
				if (zhang == 1 && jie == 3 && xinwang == 1)
				{
					draw(70, -20, &actor[zhang][jie+1]);//��ķ2
				}
				else
				{
					draw(500, 100, &actor[zhang][jie]);//��ͨ����
				}
			}
		}
	}
	draw(0, 0, &zhandoukuang);//ս����
	draw(0, 0, &xueliang);//Ѫ��ͼƬ
	draw(500, 500, &mactorshow);//���
	_itoa_s(game_hp, php, 10);
	ctot(print, php);
	outtextxy(160, 530, print);//Ѫ��
	_itoa_s(game_atk, patk, 10);
	ctot(print, patk);
	outtextxy(160, 580, print);//������
	_itoa_s(enemy_hp, php, 10);
	ctot(print, php);
	outtextxy(160, 40, print);//����Ѫ��
	draw(1050, 635, jinengnow.p);//Я���ı�������
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
	if (mouse.lbutton == 1 && (mouse.x >= 817 && mouse.x <= 1010) && (mouse.y >= 506 && mouse.y <= 605))//��ͨ����
	{
		return 1;
	}
	if (mouse.lbutton == 1 && yzzl == 0 && (mouse.x >= 1020 && mouse.x <= 1200) && (mouse.y >= 506 && mouse.y <= 605))//����֮��
	{
		return 2;
	}
	if (mouse.lbutton == 1 && cure == 0 && (mouse.x >= 817 && mouse.x <= 1010) && (mouse.y >= 615 && mouse.y <= 700))//����
	{
		return 3;
	}
	return 0;
}

void playerfight(int *player,int*flag,int *zhang,int jie,int changjing)
{
	LOGFONT f;//����������ʽ����
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
	f.lfQuality = ANTIALIASED_QUALITY;//�����
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
		*player = jinengshifang();//��ȡ���ѡ��
		flushmessage(EM_MOUSE);
		//�ж������Ϊ��
		if (*player == 1)//��ͨ����
		{
			mouse.lbutton = NULL;
			*flag = 1;
			if (*zhang == 1 && jie == 3&&weaponnow.atk==22)
			{
				PlaySound(L"��Ч/�����.wav", 0, SND_ASYNC);
				Sleep(2000);
			}
			else
			{
				PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
				srand((unsigned int)time(NULL));//������Ϊ
				int bh_2 = rand() % 100 + 1;
				if (bh_2 <= count_hunluan * 10)
				{
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
					game_hp -= game_atk;
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close BGM", 0, 0, 0);
							defeat(*zhang,jie);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							f.lfHeight = 20;
							outtextxy(0, 500, _T("��������"));
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
					mciSendString(L"close ����/ħ������.mp3", 0, 0, 0);
				}
				else
				{
					mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
				save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
				return;
			}
			if (enemy_hp <= 0 && enemy_revive == 1)
			{
				PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
				enemy_hp = 800;
				enemy_revive = 0;
				fightshow(*zhang, jie, changjing);
			}
			Sleep(250);
		}
		if (*player == 2)//����֮��
		{
			count_yzzl = 0;
			mouse.lbutton = NULL;
			yzzl = 1;
			*flag = 1;
			PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
			Sleep(50);
			for (int t = 0; t < 3; t++)
			{
				draw(360, 400, &xuli[t]);
				FlushBatchDraw();
				Sleep(80);
			}
			Sleep(500);
			PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
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
					mciSendString(L"close ����/ħ������.mp3", 0, 0, 0);
				}
				else
				{
					mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
				save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
				return;
			}
			if (enemy_hp <= 0 && enemy_revive == 1)
			{
				PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
				enemy_hp = 800;
				enemy_revive = 0;
				fightshow(*zhang, jie, changjing);
			}
			f.lfHeight = 20;
			outtextxy(0, 500, _T("��ʹ��������֮���������ӶԷ��»غϵ��ж�"));
			f.lfHeight = 45;
			FlushBatchDraw();
			Sleep(700);
		}
		if (*player == 3)
		{
			count_cure = 0;
			mouse.lbutton = NULL;
			PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
			cure = 1;
			*flag = 1;
			if (game_hp < hp)
			{
				game_hp += (hp - game_hp) / 3;//�ָ�����ʧ����ֵ������֮һ
			}
			for (int i = 0; i < 3; i++)
			{
				draw(360, 400, &cure_p[i]);
				FlushBatchDraw();
				Sleep(50);
			}
			f.lfHeight = 20;
			outtextxy(0, 500, _T("��ָ�������ʧѪ����1/3"));
			f.lfHeight = 45;
			FlushBatchDraw();
			Sleep(700);
		}
		Sleep(200);
		fightshow(*zhang,jie,changjing);
	}
	}
}

void ctolgame()//�������
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

void zhujue()//������ͼ
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

	LOGFONT f;//����������ʽ����
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
	f.lfQuality = ANTIALIASED_QUALITY;//�����
	f.lfHeight = 60;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	loadimage(&bk, L"����/�˵�.jpg", 1200, 700);
	loadimage(&cundang, L"����/�浵����.jpg", 1200, 700);
	loadimage(&back, L"����/���ذ�ť.png");
	loadimage(&statement, L"����/����.png", 1200, 700);

	mciSendString(L"open ����/�˵�.mp3 alias BGM", 0, 0, 0);
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
			outtextxy(60, 40, _T("������"));
			outtextxy(60, 350, _T("�����ˣ�"));
			outtextxy(60, 270, _T("�ر���л��"));
			outtextxy(60, 530, _T("����˵����"));
			f.lfHeight = 40;
			settextstyle(&f);
			outtextxy(260, 360, _T("UESTC-22��xxx"));
			outtextxy(260, 280, _T(" ����Ƶ��վ����̳�������UP���Ͳ���"));
			outtextxy(260, 540, _T("�ƹ�WASD�ƶ���J�����Ȧ������������"));
			f.lfHeight = 30;
			settextstyle(&f);
			outtextxy(60, 100, _T("�����������ԡ�ð�յ���,�����³�����ʿ�������������ˡ��Ⱦ�����Ϸ"));
			outtextxy(60, 140, _T("ս�����������Դ�����͵�JRPG����ʱ���һ����������ԭ��ֻ�����ǰ����"));
			outtextxy(60, 180, _T("��ͼ������������������Դ���������, ��л�������ߡ�"));
			outtextxy(60, 220, _T("����Ϸ���κ���ҵ��;����������Ϸ�����¾���"));
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
			putimage(0, 0, &cundang);//�浵ҳ�汳��ͼ
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
	TCHAR str[6];//�ַ�����
	swprintf_s(str, _T("%d"), *pn+1);
	draw((*pn+1) * 200, 350 + 150 * pow(-1, *pn), &level1);//ͼ��
	outtextxy((*pn) *200+220, 355 + 150 * pow(-1, *pn), str);//���
	(*pn)++;
}

void jiuguan(int* c)
{

	x = 570, y = 600;
	dir = 1;
	int bar = 0;
	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open ����/�ƹ�.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 500", NULL, 0, NULL);
	loadimage(&mbar, L"����/�ƹ�.jpg", 1200, 700);
	loadimage(&bar1, L"����/��̨�˵�.png", 900, 175);
	loadimage(&food1, L"����/è��.jpg", 180, 100);
	loadimage(&food2, L"����/��ά���̲�.png", 180, 100);
	loadimage(&mweapon, L"����/װ����.png");
	loadimage(&jinengstore, L"����/�����̵�.png");
	cleardevice();
	int pjiuguan = 0;//ָʾ�˳��ƹ��ñ���
	while (pjiuguan==0)
	{
		LOGFONT f;//����������ʽ����
		gettextstyle(&f);
		wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
		f.lfQuality = ANTIALIASED_QUALITY;//�����
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
		putimage(0, 0, &mbar);//�ƹݱ���
		ctolgame();//�������
		zhujue();//������ͼ
		if ((x >= 757 && x <= 805) && (y >= 426 && y <= 446))//�����¼�
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
					for (int i = 0; i < 4; i++)//iΪ����
					{
						for (int j = 0; j < 4; j++)//jΪ����
						{
							if ((4 * i + j)<wc)
							{
								draw(665+j*65,181+i*71,w[4 * i + j].p);
							}
						}
					}
					for (int i = 0; i < 4; i++)//iΪ����
					{
						for (int j = 0; j < 4; j++)//jΪ����
						{
							if (mouse.x >= 665 + j * 65 && mouse.x <= 709 + j * 65 && mouse.y >= 181 + i * 71 && mouse.y <= 228 + i * 71&&4 * i + j<wc)
							{
								ctot(print, w[4 * i + j].name);
								f.lfHeight = 15;
								outtextxy(331, 326, print);
								_itoa_s(w[4 * i + j].atk, patk, 10);
								ctot(print, patk);
								f.lfHeight = 5;
								outtextxy(331, 376, _T("��������"));
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
								for (int i = 0; i < 4; i++)//iΪ����
								{
									for (int j = 0; j < 4; j++)//jΪ����
									{
										if (mouse.x >= 665 + j * 65 && mouse.x <= 709 + j * 65 && mouse.y >= 181 + i * 71 && mouse.y <= 228 + i * 71&& 4 * i + j<wc)
										{
											atk -= weaponnow.atk;
											weaponnow = w[4 * i + j];
											atk += weaponnow.atk;
											wn = 4 * i + j;
											save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//�浵
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
		if ((x >= 201 && x <= 248) && (y >= 191 && y <= 211))//��̨�¼�
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
					outtextxy(310,650, _T("è��"));
					putimage(520, 540, &food2);
					outtextxy(530, 650, _T("��ά���̲�"));
					outtextxy(730, 580, _T("300��ҹ���"));
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
								outtextxy(550, 250, _T("����ɹ���"));
								outtextxy(550, 350, _T("������+10"));
								atk += 10;
								money -= 300;
								save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//�浵
								FlushBatchDraw();
							}
							if (money >= 300 && (mouse.x > 520 && mouse.x < 700) && (mouse.y > 541 && mouse.y < 637))
							{
								putimage(300, 175, &tishi);
								outtextxy(550, 250, _T("����ɹ���"));
								outtextxy(550, 350, _T("����ֵ+100"));
								hp += 100;
								money -= 300;
								save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//�浵
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
		if ((x >= 261 && x <= 311) && (y >= 600 && y <= 620))//ͼ���¼�
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
					for (int j = 0; j < 5; j++)//jΪ����
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
											save(re, zhangjie, game_levels, atk, hp, wc, wn,wjineng,money,newopen);//�浵
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
				if ((mouse.x > 1100 && mouse.x < 1200) && (mouse.y > 620 && mouse.y < 700))//�����½�ҳ��
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
	mciSendString(L"close ����/�ƹ�.mp3", 0, 0, 0);
	switch (*c)
	{
	case 0:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	case 1:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open ����/�ڰ�֮��3.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	case 2:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open ����/�����ƻ���3.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	case 3:
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open ����/����.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
		break;
	}
	return;
}

void game1(int zhang)//�ؿ�1
{
	cleardevice();
	flushmessage(EM_MOUSE);

	LOGFONT f;//����������ʽ����
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
	f.lfQuality = ANTIALIASED_QUALITY;//�����
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open ����/�궷��.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

	int player, enemy;//�ж���Һ͵��˵���Ϊ
	game_atk = atk;
	game_hp = hp;
	int flag;//�ж�����Ƿ����
	yzzl = 0;//����֮������ָʾ
	effect_yzzl = 0;//����֮��Ч���ж�
	cure = 0;//����ָʾ
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
	//���������жϡ���Ĭ��ȫ���رգ���Я�����
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

	int bh;//������Ϊ�����

	switch (zhang)
	{
	case 0://�ؿ�1-1
	{
		end = 0;
		enemy_hp = 200;
		enemy_atk = 15;
		putimage(0, 0, &game1_1);
		outtextxy(100, 600, _T("���ߣ��㲻֪����������ʲôѡ��,�Ҳ�������ȥð��"));
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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 80)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money, newopen);//�浵
							return;
						}
						fightshow(zhang, 0, 0);
						if (game_hp <= 0)
						{
							if (revive != 1)
							{
								mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
								defeat(zhang, 0);
								end = 1;
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money, newopen);//�浵
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						if (enemy_hp < 200)
						{
							enemy_hp += (200 - enemy_hp) / 3;//�ָ�����ʧ����ֵ������֮һ
						}
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &cure_p[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						f.lfHeight = 20;
						outtextxy(0, 500, _T("���߻ָ���һ������ֵ"));
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
					PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
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
	case 1://�ؿ�2-1
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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 80)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("���׶�������Ч"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);

					}
					fightshow(zhang, 0, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,0);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
					PlaySound(L"��Ч/��ͨ��ŭ.wav", 0, SND_ASYNC);
					enemy_atk += 20;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("�����˺������"));
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
					PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
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
	case 2://�ؿ�3-1
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
	case 3://�ؿ�4-1
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

	LOGFONT f;//����������ʽ����
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
	f.lfQuality = ANTIALIASED_QUALITY;//�����
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open ����/�궷��.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

	int player, enemy;//�ж���Һ͵��˵���Ϊ
	game_atk = atk;
	game_hp = hp;
	int flag;//�ж�����Ƿ����
	yzzl = 0;//����֮������ָʾ
	effect_yzzl = 0;//����֮��Ч���ж�
	cure = 0;//����ָʾ
	count_cure = 0;
	count_yzzl = 0;
	count_shengguang = 0;
	pkuangnu = 0;
	pzhuoshao = 0;
	pkuangnu = 0;
	enemy_revive = 0;
	pzhongdu = 0;
	end = 0;
	//���������жϡ���Ĭ��ȫ���رգ���Я�����
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

	int bh;//������Ϊ�����

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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 70)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
					}
					fightshow(zhang,1,0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,1);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang,1,0);
						}
					}
				}
				if (bh > 90&&bh<=100)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
					if (enemy_hp < 300)
					{
						enemy_hp +=(300-enemy_hp)/3;//�ָ�����ʧ����ֵ������֮һ
					}
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &cure_p[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("ţͷ�˻ָ���һ��������ֵ"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang,1,0);
				}
				if (bh > 70 && bh <= 90)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/��ͨ��ŭ.wav", 0, SND_ASYNC);
					enemy_atk += 10;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("ţͷ���˺������"));
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
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 50)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
					}
					fightshow(zhang, 1, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,1);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 1, 0);
						}
					}
				}
				if (bh > 50&&bh<=70)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
					}
					f.lfHeight = 20;
					outtextxy(0, 500, _T("������ʿʹ��������ն,�����3�δ��"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(500);
					fightshow(zhang, 1, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,1);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 1, 0);
						}
					}
				}
				if (bh > 85 && bh <= 100)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
					if (enemy_hp < 1200)
					{
						enemy_hp += 400;//�ָ�����ʧ����ֵ������֮һ
					}
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &cure_p[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("������ʿ�ָ���һ��������ֵ"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 1, 0);
				}
				if (bh > 70 && bh <= 85)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/��ͨ��ŭ.wav", 0, SND_ASYNC);
					enemy_atk += 15;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("������ʿ�˺������"));
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
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
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

	LOGFONT f;//����������ʽ����
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
	f.lfQuality = ANTIALIASED_QUALITY;//�����
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open ����/�궷��.mp3 alias BGM", 0, 0, 0);
	mciSendString(L"play BGM repeat", 0, 0, 0);
	mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

	int player, enemy;//�ж���Һ͵��˵���Ϊ
	game_atk = atk;
	game_hp = hp;
	int flag;//�ж�����Ƿ����
	yzzl = 0;//����֮������ָʾ
	effect_yzzl = 0;//����֮��Ч���ж�
	cure = 0;//����ָʾ
	count_cure = 0;
	count_yzzl = 0;
	pkuangnu = 0;
	pzhuoshao = 0;
	count_shengguang = 0;
	end = 0;
	enemy_revive = 0;
	pzhongdu = 0;
	//���������жϡ���Ĭ��ȫ���رգ���Я�����
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

	int bh;//������Ϊ�����

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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh <= 65)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
					}
					fightshow(zhang, 2,0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
					PlaySound(L"��Ч/�ж�.wav", 0, SND_ASYNC);
					for (int i = 0; i < 3; i++)
					{
						draw(360, 400, &zhongdup[i]);
						FlushBatchDraw();
						Sleep(100);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("�粼��ʹ����Ͷ�����»غ���ÿ�غϿ�ʼ�㽫�ܵ�һ���˺�"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 2,0);
				}
				if (bh > 80 && bh <= 85)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
					PlaySound(L"��Ч/��ͨ��ŭ.wav", 0, SND_ASYNC);
					enemy_atk += 15;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("�粼���˺�������"));
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
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 2,0);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("��������"));
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
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				f.lfHeight = 20;
				outtextxy(0, 500, _T("���������ǿ��"));
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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;
			if (effect_yzzl == 1)
			{
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				effect_yzzl = 0;
			}
			else
			{
				if (bh>=30&&bh <= 65)
				{
					pkuangnu = 1;
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/��ͨ��ŭ.wav", 0, SND_ASYNC);
					enemy_atk += 20;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &kuangnu[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("���״����˺�������"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(500);
					fightshow(zhang, 2, 0);
				}
				if (bh <= 50)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/sword1.wav", 0, SND_ASYNC);
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
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						enemy_hp -= enemy_atk / 3;
						if (enemy_hp <= 0)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
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
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
					}
					fightshow(zhang, 2, 0);
					if (game_hp <= 0)
					{
						if (revive != 1)
						{
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
					PlaySound(L"��Ч/�ж�.wav", 0, SND_ASYNC);
					for (int i = 0; i < 3; i++)
					{
						draw(360, 400, &zhongdup[i]);
						FlushBatchDraw();
						Sleep(100);
					}
					Sleep(200);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("���״����ͷ��˶������»غ���ÿ�غϿ�ʼ�㽫�ܵ�һ���˺�"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(700);
					fightshow(zhang, 2, 0);
				}
				if (bh > 65 && bh <= 75)
				{
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
					enemy_hp +=400;
					for (int i = 0; i < 3; i++)
					{
						draw(360, 100, &cure_p[i]);
						FlushBatchDraw();
						Sleep(50);
					}
					f.lfHeight = 20;
					outtextxy(0, 500, _T("���״���������Ѫ��"));
					f.lfHeight = 45;
					FlushBatchDraw();
					Sleep(500);
					fightshow(zhang, 2, 0);
				}
				if (bh > 90 && bh <= 100)//����
				{
					phunluan = 1;
					count_hunluan+=3;
					mouse.lbutton = NULL;
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
					for (int i = 0; i < 3; i++)
					{
						draw(360, 400, &hunluan[i]);
						FlushBatchDraw();
						Sleep(100);
					}
					f.lfHeight = 20;
					outtextxy(0, 500, _T("���״�������Ӱ����������ǣ����´ι����нϴ���ʶ��Լ���ɵ����˺�"));
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
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,2);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 2, 0);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("��������"));
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
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				f.lfHeight = 20;
				outtextxy(0, 500, _T("���������ǿ��"));
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

	LOGFONT f;//����������ʽ����
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
	f.lfQuality = ANTIALIASED_QUALITY;//�����
	f.lfHeight = 45;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(WHITE);
	settextstyle(&f);

	int player, enemy;//�ж���Һ͵��˵���Ϊ
	game_atk = atk;
	game_hp = hp;
	int flag;//�ж�����Ƿ����
	yzzl = 0;//����֮������ָʾ
	effect_yzzl = 0;//����֮��Ч���ж�
	cure = 0;//����ָʾ
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

	//���������жϡ���Ĭ��ȫ���رգ���Я�����
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

	int bh;//������Ϊ�����

	switch (zhang) {//�ж��½�
	case 0:
	{
		mciSendString(L"close BGM", 0, 0, 0);
		mciSendString(L"open ����/ħ������.mp3 alias BGM", 0, 0, 0);
		mciSendString(L"play BGM repeat", 0, 0, 0);
		mciSendString(L"setaudio BGM volume to 150", NULL, 0, NULL);

		int lhhx = 1;
		enemy_hp = 1500;
		enemy_atk = 30;
		putimage(0, 0, &game1_boss);
		outtextxy(300, 600, _T("���ǣ��Ŷ�����������ʲô�أ�"));
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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;
			if (enemy_hp < 400 && lhhx == 1)
			{
				lhhx = 0;
				PlaySound(L"��Ч/������.wav", 0, SND_ASYNC);
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
				outtextxy(0, 500, _T("�Ŷ���ʹ������������͵ȡ�˲�������ֵ����������Ĺ�����"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(700);
				fightshow(zhang, 3, 1);
			}
			else
			{
				if (effect_yzzl == 1)
				{
					PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
					effect_yzzl = 0;
				}
				else
				{
					if (bh <= 65)//�Ŷ�����ͨ����
					{
						mouse.lbutton = NULL;
						PlaySound(L"��Ч/�Ŷ�����ͨ����.wav", 0, SND_ASYNC);
						Sleep(100);
						for (int t = 0; t < 3; t++)
						{
							draw(800 - 150 * t, 100 + 250 * t, &fireball[t]);
							FlushBatchDraw();
							Sleep(150);
							fightshow(zhang, 3, 1);
						}
						PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
						for (int t = 0; t < 3; t++)
						{
							draw(500, 600, &boom[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						game_hp -= enemy_atk;
						if (fanjia == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							enemy_hp -= enemy_atk / 3;
							if (enemy_hp <= 0)
							{
								mciSendString(L"close ����/ħ������.mp3", 0, 0, 0);
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
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
								return;
							}
							f.lfHeight = 20;
							outtextxy(0, 500, _T("���״���"));
							f.lfHeight = 45;
							FlushBatchDraw();
							Sleep(500);
						}
						fightshow(zhang, 3, 1);
						if (game_hp <= 0)
						{
							if (revive != 1)
							{
								mciSendString(L"close ����/ħ������.mp3", 0, 0, 0);
								defeat(zhang,3);
								end = 1;
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
								f.lfHeight = 20;
								outtextxy(0, 500, _T("��������"));
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
					if (bh > 65 && bh <= 70)//�Ŷ�����Ѫ
					{
						mouse.lbutton = NULL;
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						if (enemy_hp < 1500)
						{
							enemy_hp += 500;//�ָ�500Ѫ
						}
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &cure_p[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("�Ŷ����ָ��˲���Ѫ��"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(700);
						fightshow(zhang, 3, 1);
					}
					if (bh > 70 && bh <= 80)//����
					{
						phunluan = 1;
						count_hunluan++;
						mouse.lbutton = NULL;
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						for (int i = 0; i < 3; i++)
						{
							draw(360, 400, &hunluan[i]);
							FlushBatchDraw();
							Sleep(100);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("�Ŷ���ʹ���˻���,���´ι����и��ʶ��Լ���ɵ����˺�"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(700);
						fightshow(zhang, 3, 1);
					}
					if (bh > 80 && bh <= 90)
					{
						mouse.lbutton = NULL;
						pzhongdu++;
						PlaySound(L"��Ч/�ж�.wav", 0, SND_ASYNC);
						for (int i = 0; i < 3; i++)
						{
							draw(360, 400, &zhongdup[i]);
							FlushBatchDraw();
							Sleep(100);
						}
						f.lfHeight = 20;
						outtextxy(0, 500, _T("�Ŷ����ͷ��˶���,�»غϿ�ʼÿ�غ��㽫�ܵ�һ���˺�"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);
						fightshow(zhang, 3, 1);
					}
					if (bh > 90 && bh <= 100)
					{
						pkuangnu = 1;
						mouse.lbutton = NULL;
						PlaySound(L"��Ч/��ͨ��ŭ.wav", 0, SND_ASYNC);
						enemy_atk += 15;
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &kuangnu[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("�Ŷ��������˹�����"));
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
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,3);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang,3, 1);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("��������"));
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
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				game_hp += 10;
				game_atk += 10;
				f.lfHeight = 20;
				outtextxy(0, 500, _T("������Եõ���ǿ"));
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
		mciSendString(L"open ����/��ķ.mp3 alias BGM", 0, 0, 0);
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
			outtextxy(300, 600, _T("Ըн����̫��ͬ��"));
		}
		else
		{
			outtextxy(300, 600, _T("��������ŵ����"));
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

			srand((unsigned int)time(NULL));//������Ϊ
			bh = rand() % 100 + 1;

			if (strongatk == 1)//ǿ��һ��
			{
				if (weaponnow.atk == 22&&effect_yzzl!=1)
				{
					PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
					PlaySound(L"��Ч/�����.wav", 0, SND_ASYNC);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("ǿ��һ����������,��Ϊ�籩֮��"));
					Sleep(3000);
					f.lfHeight = 45;
					FlushBatchDraw();
					strongatk = 0;
				}
				if (weaponnow.atk == 22 && effect_yzzl == 1)
				{
					PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
					PlaySound(L"��Ч/�����.wav", 0, SND_ASYNC);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("ǿ��һ����������,��Ϊ�籩֮��"));
					f.lfHeight = 45;
					FlushBatchDraw();
					strongatk = 0;
				}
				if (effect_yzzl == 1 && weaponnow.atk != 22)
				{
					PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
					f.lfHeight = 20;
					outtextxy(0, 500, _T("ǿ��һ����������,��Ϊ����֮��"));
					f.lfHeight = 45;
					FlushBatchDraw();
					strongatk = 0;
					effect_yzzl = 0;
				}
				Sleep(500);

				if (strongatk == 1)
				{
					strongatk = 0;

					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
					PlaySound(L"��Ч/������.wav", 0, SND_ASYNC);
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
					PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);

					pzhuoshao = 1;//����Ч��

					mouse.lbutton = NULL;
					PlaySound(L"��Ч/����������.wav", 0, SND_ASYNC);
					Sleep(3000);
					PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
					Sleep(300);
					PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
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
					outtextxy(0, 500, _T("��������ķ�ͷ���ǿ��һ��"));
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
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 3, 1);
						}
					}
				}
			}

			if (enemy_hp < 4000 && xinwang == 0)
			{
				xinwang = 1;//������׶�
				strongatk = 1;
				enemy_atk += 200;

				mciSendString(L"close BGM", 0, 0, 0);
				mciSendString(L"open ����/��������ķ.mp3 alias BGM", 0, 0, 0);
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
				outtextxy(0, 500, _T("��ķ������н��״̬��������ǿ��������һ��ǿ������"));
				f.lfHeight = 45;
				FlushBatchDraw();
				Sleep(1500);
				fightshow(zhang, 3, 1);

			}
			else
			{
				if (effect_yzzl == 1)
				{
					PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
					Sleep(500);
					fightshow(zhang, 3, 1);
					effect_yzzl = 0;
				}
				else
				{
					if (bh <= 75)
					{
						mouse.lbutton = NULL;
						PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
						for (int t = 0; t < 3; t++)
						{
							draw(500, 600, &boom[t]);
							FlushBatchDraw();
							Sleep(50);
						}
						game_hp -= enemy_atk;
						if (fanjia == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
								return;
							}
							f.lfHeight = 20;
							outtextxy(0, 500, _T("���״���"));
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
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
								f.lfHeight = 20;
								outtextxy(0, 500, _T("��������"));
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
					if (bh > 75 && bh <= 80)//��������Ѫ
					{
						mouse.lbutton = NULL;
						PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
						if (enemy_hp < 9999)
						{
							enemy_hp += 1000;//�ָ�1000Ѫ
						}
						for (int i = 0; i < 3; i++)
						{
							draw(360, 100, &cure_p[i]);
							FlushBatchDraw();
							Sleep(50);
						}
						Sleep(200);
						f.lfHeight = 20;
						outtextxy(0, 500, _T("�������ָ��˲���Ѫ��"));
						f.lfHeight = 45;
						FlushBatchDraw();
						Sleep(500);
						fightshow(zhang, 3, 1);
					}
					if (bh > 80 && bh <= 100)
					{
						mouse.lbutton = NULL;
						PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
						Sleep(300);
						PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
						Sleep(300);
						PlaySound(L"��Ч/��ը.wav", 0, SND_ASYNC);
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
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
								return;
							}
						}
						f.lfHeight = 20;
						outtextxy(0, 500, _T("������ʹ������������"));
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
								save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
								return;
							}
							if (revive == 1)
							{
								PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
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
							mciSendString(L"close ����/�궷��.mp3", 0, 0, 0);
							defeat(zhang,3);
							end = 1;
							save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
							return;
						}
						if (revive == 1)
						{
							PlaySound(L"��Ч/����.wav", 0, SND_ASYNC);
							game_hp = hp;
							revive = 0;
							fightshow(zhang, 3, 1);
							pzhongdu = 0;
							f.lfHeight = 20;
							outtextxy(0, 500, _T("��������"));
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
				PlaySound(L"��Ч/ʥ��.wav", 0, SND_ASYNC);
				game_hp += 20;
				game_atk += 20;
				f.lfHeight = 20;
				outtextxy(0, 500,_T("������ǿ"));
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
	LOGFONT f;//����������ʽ����
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("����Ҧ��"));
	f.lfQuality = ANTIALIASED_QUALITY;//�����
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
	putimage(0, 0, &mb[c]);//����浵�Զ�����һ�½���
	draw(1100, 0, &barp);
	if (c < zhangjie-1)
	{
		draw(1100, 300, &directR);//������ͷ
	}
	if (c > 0&&c<=zhangjie-1)
	{
		draw(0, 300, &directL);//������ͷ
	}
	if (c == zhangjie-1)
	{
		while (nz < game_levels&&nz!=3)//���ؿ�
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
		while (nz < 3)//���ؿ�
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
			if (mouse.lbutton && (mouse.x > 1100 && mouse.x < 1189) && (mouse.y > 0 && mouse.y < 89))//����ƹ�
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
					mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�ڰ�֮��3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�����ƻ���3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����.mp3 alias BGM", 0, 0, 0);
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
					mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�ڰ�֮��3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�����ƻ���3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
				flushmessage(EM_MOUSE);
			}
			*change = c;

			if ((c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 1)) && mouse.lbutton && (mouse.x > 200 && mouse.x < 264) && (mouse.y > 500 && mouse.y < 564))//�����һ��
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				game1(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�ڰ�֮��3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�����ƻ���3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
			}

			if ((c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 2)) && mouse.lbutton && (mouse.x > 400 && mouse.x < 464) && (mouse.y > 200 && mouse.y < 264))//����ڶ���
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				game2(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�ڰ�֮��3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�����ƻ���3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
			}

			if ((c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 3)) && mouse.lbutton && (mouse.x > 600 && mouse.x < 664) && (mouse.y > 500 && mouse.y < 564))//���������
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				game3(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�ڰ�֮��3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�����ƻ���3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				}
			}

			if ((game_levels==4||c<zhangjie)&&(c < zhangjie - 1 || (c == zhangjie - 1 && game_levels >= 4)) && mouse.lbutton && (mouse.x > 800 && mouse.x < 864) && (mouse.y > 200 && mouse.y < 264))//����BOSS��
			{
				flushmessage(EM_MOUSE);
				mouse.lbutton = NULL;
				gameboss(c);
				switch (c)
				{
				case 0:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 1:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�ڰ�֮��3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 2:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/�����ƻ���3.mp3 alias BGM", 0, 0, 0);
					mciSendString(L"play BGM repeat", 0, 0, 0);
					mciSendString(L"setaudio BGM volume to 800", NULL, 0, NULL);
					break;
				case 3:
					mciSendString(L"close BGM", 0, 0, 0);
					mciSendString(L"open ����/����.mp3 alias BGM", 0, 0, 0);
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
	FILE* file = NULL;//�����ļ�
	int error;//������Ϣ
	int change = 0;//���½��ñ���
	int* pc = &change;//���½���ָ��
	loadimage(&barp, L"ͼ��/�ƹ�ͼ��.png");
	loadimage(&level1, L"ͼ��/��һ��ͼ��.png");
	loadimage(&level1BOSS, L"ͼ��/��һ��ͼ��BOSS.png");
	loadimage(&directR, L"����/�Ҽ�ͷ.png",100,100);
	loadimage(&directL, L"����/���ͷ.png",100,100);
	loadimage(&mb[0], L"����/�İ�����.jpg", 1200, 700);
	loadimage(&mb[1], L"����/��ҵ֮��.jpg", 1200, 700);
	loadimage(&mb[2], L"����/��֮����.jpg", 1200, 700);
	loadimage(&mb[3], L"����/���֮��.jpg", 1200, 700);
	loadimage(&gback, L"����/��Ϸ�ﷵ��.png");
	loadimage(&tishi, L"����/��ʾ.jpg");
	if (re == 1)
	{
		error = fopen_s(&file,"�浵/�浵һ.txt", "r");
	}
	if (re == 2)
	{
		error = fopen_s(&file,"�浵/�浵��.txt", "r");
	}
	if (re == 3)
	{
		error = fopen_s(&file,"�浵/�浵��.txt", "r");
	}
	if (error != 0)
	{
		exit(0);
	}
	fscanf_s(file, "%d\n", &zhangjie);//��ȡ��Ϸ�½�
	fscanf_s(file, "%d\n", &game_levels);//��ȡ��Ϸ�½��ڵĹؿ�״��
	fscanf_s(file, "%d %d\n", &hp, &atk);//��ȡ����ֵ�͹�����
	fscanf_s(file, "%d\n", &wc);//��ȡ�ѻ������
	fscanf_s(file, "%d\n", &wn);//����װ�����������
	fscanf_s(file, "%d\n", &wjineng);//����װ���ļ��ܱ��
	fscanf_s(file, "%d\n", &money);//����װ�����������
	fscanf_s(file, "%d\n", &newopen);//�Ƿ��һ����
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
		save(re, zhangjie, game_levels, atk, hp, wc, wn, wjineng, money,newopen);//�浵
		Sleep(8000);
	}

	mciSendString(L"close BGM", 0, 0, 0);
	mciSendString(L"open ����/����֮ɭ.mp3 alias BGM", 0, 0, 0);
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
	peekmessage(&mouse, EX_MOUSE);//����ȡ��Ϣ����
	setbkcolor(WHITE);

	HCURSOR hcur = LoadCursorFromFile(TEXT("ͼ��/���.cur"));//�����������ʽ
	HWND hwnd = GetHWnd();
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur);

	IMAGE tips;//������ʾ
	loadimage(&tips, L"����/Tips.png",1200,700);
	putimage(0, 0, &tips);
	Sleep(3000);

	int duqu=0;//ѡ��浵������
	re = menu();
	while (1)
	{
		game(re);
	}
	system("pause");
	return 0;
}