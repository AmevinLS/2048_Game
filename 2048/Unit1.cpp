//---------------------------------------------------------------------------

#include <vcl.h>
#include <string>
#include <algorithm>
#include <string>
#include <time.h>
#include <fstream>
#include "sources/game2048.cpp"
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
Game game(4);
vector<vector<tuple<int,int>>> field_coords(4, vector<tuple<int,int>>(4));
Graphics::TBitmap* ris1 = new Graphics::TBitmap();
Graphics::TBitmap* ris2 = new Graphics::TBitmap();
Graphics::TBitmap* field_ris = new Graphics::TBitmap();
Graphics::TBitmap* num_ris = new Graphics::TBitmap();
Graphics::TBitmap* temp_field_ris = new Graphics::TBitmap();
TRect num_rect, temp_num_rect, im_rect;

vector<tuple<int,int>> last_double;
vector<int> last_double_num;
vector<tuple<tuple<int,int>,tuple<int,int>>> last_trans;
tuple<int,int> last_added;
vector<int> curr_x, curr_y, target_x, target_y, start_x, start_y, dist;
vector<bool> finished;
int curr_score = 0, high_score;
int w, h, vel_x, vel_y;
int VEL = 20;

ifstream fin("high_score.txt");
ofstream fout;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	srand(time(NULL));
	game.restart();
	Image1->Picture->Bitmap->LoadFromFile("images/field_4x4.bmp");
	field_ris->LoadFromFile("images/field_4x4.bmp");
	Label2->Caption = "Score:\n0";
	fin >> high_score;
	fin.close();
	Label3->Caption = "High Score:\n" + IntToStr(high_score);
	int delta = 113;//Image1->Width / 4 + 5;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			field_coords[i][j] = make_tuple(14+j*delta, 14+i*delta);
		}
	}

	vector<tuple<int,int>> start_cells = game.get_start_cells();
	ris1->LoadFromFile("images/2.bmp");
	w = ris1->Width;
	h = ris1->Height;
	num_rect = Bounds(0,0,w,h);
	num_ris->Width = w;
	num_ris->Height = h;
	temp_field_ris->Width = w;
    temp_field_ris->Height = h;
	for(auto& start_cell:start_cells)
	{
		int im_x = get<0>(field_coords[get<0>(start_cell)][get<1>(start_cell)]);
		int im_y = get<1>(field_coords[get<0>(start_cell)][get<1>(start_cell)]);
		im_rect = Bounds(im_x, im_y, w, h);
		Image1->Canvas->CopyRect(im_rect, ris1->Canvas, num_rect);
	}
}
//---------------------------------------------------------------------------
void to_screen_coords()
{
	curr_x.clear();
	curr_y.clear();
	target_x.clear();
	target_y.clear();
	dist.clear();
	dist = vector<int>(last_trans.size(), 0);
    finished = vector<bool>(last_trans.size(), false);
	for(int i=0;i<last_trans.size();i++)
	{
		auto trans = last_trans[i];
		curr_x.push_back(get<0>(field_coords[get<0>(get<0>(trans))][get<1>(get<0>(trans))]));
		curr_y.push_back(get<1>(field_coords[get<0>(get<0>(trans))][get<1>(get<0>(trans))]));
		target_x.push_back(get<0>(field_coords[get<0>(get<1>(trans))][get<1>(get<1>(trans))]));
		target_y.push_back(get<1>(field_coords[get<0>(get<1>(trans))][get<1>(get<1>(trans))]));
	}
	start_x = curr_x;
    start_y = curr_y;
}

void __fastcall TForm1::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
    if(Timer1->Enabled || (game.get_state()!=ONGOING))
		return;
	string s;
	vel_x = 0;
	vel_y = 0;
	switch(Key)
	{
	case 'a':
		game.make_move(LEFT);
		vel_x = -VEL;
		break;
	case 'd':
		game.make_move(RIGHT);
		vel_x = VEL;
		break;
	case 'w':
		game.make_move(UP);
		vel_y = -VEL;
		break;
	case 's':
		game.make_move(DOWN);
		vel_y = VEL;
		break;
	default:
		return;
		break;
	}

	if(RadioGroup1->Enabled)
	{
		int t_num = int(StrToFloat(RadioGroup1->Items->Strings[RadioGroup1->ItemIndex]));
		game.set_target_num(t_num);
		RadioGroup1->Enabled = false;
	}

	last_double = game.get_last_double();
	last_trans = game.get_last_trans();
	last_added = game.get_last_added();
	last_double_num = game.get_last_double_num();

	to_screen_coords();
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	bool moved;
		moved = false;
		for(int i=0;i<last_trans.size();i++)
		{
			if(finished[i])
                continue;
			if(dist[i]+VEL > abs(target_x[i]-start_x[i]+target_y[i]-start_y[i]))
			{
                temp_num_rect = Bounds(curr_x[i],curr_y[i],w,h);
				num_ris->Canvas->CopyRect(num_rect, Image1->Canvas, temp_num_rect);
				temp_field_ris->Canvas->CopyRect(num_rect, field_ris->Canvas, temp_num_rect);

				Image1->Canvas->Draw(curr_x[i], curr_y[i], temp_field_ris);

				Image1->Canvas->Draw(target_x[i], target_y[i], num_ris);
                finished[i] = true;
				continue;
			}
			temp_num_rect = Bounds(curr_x[i],curr_y[i],w,h);
			num_ris->Canvas->CopyRect(num_rect, Image1->Canvas, temp_num_rect);
			temp_field_ris->Canvas->CopyRect(num_rect, field_ris->Canvas, temp_num_rect);

			Image1->Canvas->Draw(curr_x[i], curr_y[i], temp_field_ris);

			curr_x[i] += vel_x;
			curr_y[i] += vel_y;

            Image1->Canvas->Draw(curr_x[i], curr_y[i], num_ris);

			moved = true;
			dist[i] += VEL;
		}
	if(!moved)
	{
		Timer1->Enabled = false;

        ris1->LoadFromFile("images/2.bmp");
		int added_i, added_j, added_x, added_y;
		added_i = get<0>(last_added);
		added_j = get<1>(last_added);
		added_x = get<0>(field_coords[added_i][added_j]);
		added_y = get<1>(field_coords[added_i][added_j]);
		Image1->Canvas->Draw(added_x,added_y,ris1);

        int double_i, double_j, double_x, double_y;
		for(int i=0;i<last_double_num.size();i++)
		{
			ris1->LoadFromFile("images/"+IntToStr(last_double_num[i])+".bmp");
			double_i = get<0>(last_double[i]);
			double_j = get<1>(last_double[i]);
			double_x = get<0>(field_coords[double_i][double_j]);
			double_y = get<1>(field_coords[double_i][double_j]);
			Image1->Canvas->Draw(double_x, double_y, ris1);
            curr_score += last_double_num[i];
		}

		Label2->Caption = "Score:\n" + IntToStr(curr_score);
		if(curr_score > high_score)
		{
			high_score = curr_score;
			Label3->Caption = "High Score:\n" + IntToStr(high_score);
			ofstream fout("high_score.txt");
			fout << high_score;
			fout.close();
        }

		if(game.get_state()==WON)
		{
			ris2->LoadFromFile("images/you_win.bmp");
			Image1->Canvas->Draw(0, 0, ris2, 200);
		}
		else if(game.get_state()==LOST)
		{
			ris2->LoadFromFile("images/game_over.bmp");
			Image1->Canvas->Draw(0, 0, ris2, 200);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	RadioGroup1->Enabled = true;
	game.restart();

	curr_score = 0;
    Label2->Caption = "Score:\n0";
	Image1->Canvas->Draw(0,0,field_ris);
	vector<tuple<int,int>> start_cells = game.get_start_cells();
	ris1->LoadFromFile("images/2.bmp");
	for(auto& start_cell:start_cells)
	{
		int im_x = get<0>(field_coords[get<0>(start_cell)][get<1>(start_cell)]);
		int im_y = get<1>(field_coords[get<0>(start_cell)][get<1>(start_cell)]);
		im_rect = Bounds(im_x, im_y, w, h);
		Image1->Canvas->CopyRect(im_rect, ris1->Canvas, num_rect);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Label3->Caption = "High Score:\n0";
	ofstream fout("high_score.txt");
    high_score = 0;
	fout << 0;
	fout.close();
}
//---------------------------------------------------------------------------

