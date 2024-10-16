#include "GUIMyFrame1.h"
#include <cstdlib>

#define SIDE 60

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
	bool IsColor = false;
	function_number = 0;
    PrepareData(function_number);
    PreperPoints();
}

void GUIMyFrame1::UpdateUI( wxUpdateUIEvent& event )
{
	Repaint();
}

void GUIMyFrame1::m_button1_click( wxCommandEvent& event )
{
	function_number = 1;
    PrepareData(function_number);
    PreperPoints();
	Repaint();
}

void GUIMyFrame1::m_button2_click( wxCommandEvent& event )
{
	function_number = 2;
    PrepareData(function_number);
    PreperPoints();
	Repaint();
}

void GUIMyFrame1::m_button3_click( wxCommandEvent& event )
{
	function_number = 3;
    PrepareData(function_number);
    PreperPoints();
	Repaint();
}

void GUIMyFrame1::m_button4_click( wxCommandEvent& event )
{
	function_number = 4;
    PrepareData(function_number);
    PreperPoints();
	Repaint();
}

void GUIMyFrame1::m_cb_color_click( wxCommandEvent& event )
{
	if (m_cb_color->IsChecked())
	{
		IsColor = true;
	}
	else
	{
		IsColor = false;
	}
	Repaint();
}

void GUIMyFrame1::m_s_rotation_onscroll( wxScrollEvent& event )
{
    m_st_rotation->SetLabel(wxString::Format("Obrot: %d stopni.",m_s_rotation->GetValue()));
    RotatePoints(m_s_rotation->GetValue());
    Repaint();
}

void GUIMyFrame1::m_s_tilt_onscroll( wxScrollEvent& event )
{
    Repaint();
}

void GUIMyFrame1::Repaint()
{
    wxClientDC dc1(m_panel1);
    wxBufferedDC dc(&dc1);
    dc.SetBackground(wxBrush(wxColor(255, 255, 255)));
    dc.Clear();

    int width = m_panel1->GetSize().GetWidth();
    int high = m_panel1->GetSize().GetHeight();

    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            wxColor color=PickColor(Data[i][j]);
            //VoxelSpace(i, j, Data[i][j], m_s_tilt->GetValue()/100.0, high / ((max_value - mini_value) * 5), color, dc);
            VoxelSpace(i + (SIDE - j) / 3.0, j, Data[i][j], (m_s_tilt->GetValue() + 30.0) / 100.0, high / ((max_value - mini_value) * 5), color, dc);
        }
    }
}


void GUIMyFrame1::PrepareData(int fun)
{
    int i;
    float x, y;
    srand(376257);

    switch (fun)
    {
    case 0: NoPoints = 5;
        FunctionData[0][0] = -1.0;	FunctionData[0][1] = 1.0;	FunctionData[0][2] = 0.0;
        FunctionData[1][0] = 1.0;	FunctionData[1][1] = 1.0;	FunctionData[1][2] = 25.0;
        FunctionData[2][0] = 1.0;	FunctionData[2][1] = -1.0;	FunctionData[2][2] = 5.0;
        FunctionData[3][0] = -1.0;	FunctionData[3][1] = -1.0;	FunctionData[3][2] = 25.0;
        FunctionData[4][0] = 0.0;	FunctionData[4][1] = 0.0;	FunctionData[4][2] = 15.0;
        break;
    case 1: NoPoints = 20;
        for (i = 0; i < NoPoints; i++)
        {
            x = 4.8 * (float(rand()) / RAND_MAX) - 2.4;
            y = 4.8 * (float(rand()) / RAND_MAX) - 2.4;
            FunctionData[i][0] = x;
            FunctionData[i][1] = y;
            FunctionData[i][2] = 200 * (float(rand()) / RAND_MAX);
        }
        break;
    case 2: NoPoints = 100;
        for (i = 0; i < NoPoints; i++)
        {
            x = 4.8 * (float(rand()) / RAND_MAX) - 2.4;
            y = 4.8 * (float(rand()) / RAND_MAX) - 2.4;
            FunctionData[i][0] = x;
            FunctionData[i][1] = y;
            FunctionData[i][2] = x * (2 * x - 7) * (2 * y + 1) + 2 * y;
        }
        break;
    case 3: NoPoints = 10;
        for (i = 0; i < NoPoints; i++)
        {
            x = 4.8 * (float(rand()) / RAND_MAX) - 2.4;
            y = 4.8 * (float(rand()) / RAND_MAX) - 2.4;
            FunctionData[i][0] = x;
            FunctionData[i][1] = y;
            FunctionData[i][2] = x * sin(2 * y);
        }
        break;
    case 4: NoPoints = 100;
        for (i = 0; i < NoPoints; i++)
        {
            x = 2 * (float(rand()) / RAND_MAX) - 1;
            y = 2 * (float(rand()) / RAND_MAX) - 1;
            FunctionData[i][0] = x;
            FunctionData[i][1] = y;
            FunctionData[i][2] = sin(8 * (x * x + y * y));
        }
        break;
    }
}

float GUIMyFrame1::Shepard(int number_of_poits, float d[][3], float x, float y)
{
    float denominator = 0, numerator = 0;

    if (function_number == 4)
    {
        x = x * 3.0 / SIDE - 1.5;
        y = y * 3.0 / SIDE - 1.5;
    }
    else
    {
        x = x * 5.0 / SIDE - 2.5;
        y = y * 5.0 / SIDE - 2.5;
    }
    

    for (size_t i = 0; i < number_of_poits; i++)
    {
        float X_k_value = d[i][0];
        float Y_k_value = d[i][1];
        float Z_k_value = d[i][2];

        if (fabs(X_k_value - x) < EPSILON && fabs(Y_k_value - y) < EPSILON)
        {
            return Z_k_value;
        }

        float f_weight_k = 1.0 / sqrt((x - X_k_value) * (x - X_k_value) + (y - Y_k_value) * (y - Y_k_value));
        f_weight_k = f_weight_k * f_weight_k; //P=2

        denominator += f_weight_k;
        numerator += f_weight_k * Z_k_value;
    }

    return numerator / denominator;
}

void GUIMyFrame1::VoxelSpace(float Xcor, float Ycor, float high, float horizon, float high_scale, wxColor color, wxBufferedDC& dc)
{
    if (function_number == 4)
    {
        Xcor = Xcor * 3.0 / SIDE - 1.5;
        Ycor = Ycor * 3.0 / SIDE - 1.5;
    }
    else
    {
        Xcor = Xcor * 5.0 / SIDE - 2.5;
        Ycor = Ycor * 5.0 / SIDE - 2.5;
    }

    Xcor *= 6;
    Ycor *= 5;
    Ycor *= horizon;

    if (function_number == 4)
    {
        Xcor = (Xcor + 1.5) * SIDE / 3.0;
        Ycor = (Ycor + 1.5) * SIDE / 3.0;
    }
    else
    {
        Xcor = (Xcor + 2.5) * SIDE / 5.0;
        Ycor = (Ycor + 2.5) * SIDE / 5.0;
    }
    

    int panel_width = m_panel1->GetSize().GetWidth();
    int panel_high = m_panel1->GetSize().GetHeight();
    
    Xcor += (panel_width - SIDE) / 2.7;
    Ycor += (panel_high - SIDE) / 1.65;
    dc.SetPen(wxPen(color, 9));
    dc.DrawLine(Xcor, Ycor, Xcor, Ycor - high * high_scale);

}

wxColor GUIMyFrame1::PickColor(float high)
{
    float scale_color = (high - mini_value) / (max_value - mini_value);
    scale_color *= 255;
    wxColor picked;
    if (IsColor)
        picked = wxColor(scale_color, 0, 255 - scale_color);
    else
        picked = wxColor(scale_color, scale_color, scale_color);

    return picked;
}

void GUIMyFrame1::PreperPoints()
{
    mini_value = 1000;
    max_value = -1000;

    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            Data[i][j] = Shepard(NoPoints, FunctionData, i, j);
            if (Data[i][j] > max_value)
                max_value = Data[i][j];
            if (Data[i][j] < mini_value)
                mini_value = Data[i][j];
        }
    }


    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            Data[i][j] -= mini_value;
        }
    }
    max_value -= mini_value;
    mini_value = 0;
}

void GUIMyFrame1::RotatePoints(double alfa)
{
    PrepareData(function_number);
    alfa = (alfa / 180.0) * M_PI;
    for (int i = 0; i < NoPoints; i++)
    {
        float Xcor = FunctionData[i][0] * cos(alfa) - FunctionData[i][1] * sin(alfa);
        float Ycor = FunctionData[i][0] * sin(alfa) + FunctionData[i][1] * cos(alfa);
        
        FunctionData[i][0] = Xcor;
        FunctionData[i][1] = Ycor;
    }
    PreperPoints();
}
