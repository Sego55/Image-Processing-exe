#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

/**********************************************/
/******************Struct*******************/
struct Pixel
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
};
/**********************************************/
class Image
{
private:
    int MAX;
    char m1;
    char m2;
    unsigned int width;
    unsigned int height;
    unsigned int colorValue;
    Pixel **r;
    ///////////////////////////////////////////////////
    void alloc_raster()
    {
        if (r != nullptr)
            delete r;
        if (width > 0 && height > 0)
        {
            r = new Pixel *[height];                  // allocate pointers to the rows
            for (unsigned int k = 0; k < height; ++k) // for all rows
            {
                r[k] = new Pixel[width]; // allocate pixels in row
            }
        }
        else
        {
            cerr << "Width or Height or both are Negative failed to intilazie !!!" << endl;
        }
    };

public:
    /*****************Con&Decon*************************/
    Image();
    /**********************************************/
    void read_ppm(string ext);
    void write_ppm(string ext);
    void mirror_vertically();
    void sepia();
    void strip_curtain(int curtain_W, int dis_curtain, int c_R, int c_G, int c_B);
    /**********************************************/
};
Image::Image()
{
    /* constructor is intializing the data*/
    this->m1 = 'P';
    this->m2 = '3';
    this->width = 0;
    this->height = 0;
    this->colorValue = 255;
    this->MAX = 255;
    this->r = nullptr;
};
/**********************************************/
void Image::read_ppm(string ext)
{
    fstream f_Read;            // file stream object
    unsigned int no_Line = 0;  // track of index while loop
    f_Read.open(ext, ios::in); // open file in read mode
    if (f_Read.is_open())      // check if the file is open
    {
        string line;
        unsigned int row_index = 0; // row and column index
        unsigned int col_index = 0;
        while (getline(f_Read, line)) // get file line by line
        {
            istringstream ss(line); // stream the line word by word
            switch (no_Line)
            {
            case 0: // in case first line
                cout << "           read header of file...\n";
                ss >> this->m1 >> this->m2; // assign them to magic numbers
                no_Line++;
                break;
            case 1:                                // in case 2nd line
                ss >> this->width >> this->height; // assign the height and width
                cout << "           allocate " << width << "x" << height << " pixel array on heap... done" << endl;
                no_Line++;
                break;
            case 2: // 3rd Line ColorValue
                ss >> this->colorValue;
                no_Line++;
                break;
            case 3: // comment line ignore
                /*The Header line*/
                cout << "           read RGB pixel data row by row..." << endl;
                alloc_raster(); // allocation for my RGB Array
                no_Line++;
                break;
            default:                                                                                                         /*after Header line*/
                ss >> this->r[row_index][col_index].r >> this->r[row_index][col_index].g >> this->r[row_index][col_index].b; // assign every r g b value to its place
                if (col_index == (width - 1))                                                                                // if col_index equals the width -1 that means that we should assign to the next row
                {
                    /*cout << line << "        [" << row_index << "]" // Just to clarify
                         << "[" << col_index << "]" << endl; */

                    // next row
                    col_index = 0; // start col from 0;
                    row_index++;
                }
                else
                {
                    /*
                    cout << line << "        [" << row_index << "]" // Just to clarify
                         << "[" << col_index << "]" << endl; */
                    col_index++; // next colmun
                }
            }
        }
        f_Read.close(); // close file
        cout << "           done and" << ext << " closed" << endl;
    };
}
/**********************************************/
void Image::write_ppm(string ext)
{
    fstream f_write;
    unsigned int no_Line = 0;
    f_write.open(ext, ios::out);
    cout << "write_ppm:" << ext << " opened..." << endl;
    if (f_write.is_open())
    {
        cout << "           write header of file..." << endl;
        f_write << this->m1 << this->m2 << endl;
        f_write << this->width << " " << this->height << endl;
        f_write << this->colorValue << endl;
        f_write << "# end of header" << endl;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                f_write << this->r[i][j].r << " " << this->r[i][j].g << " " << this->r[i][j].b << endl;
            }
        }
        f_write.close();
    }
    cout << "           done and " << ext << " closed" << endl;
};
/**********************************************/
void Image::mirror_vertically()
{
    cout << "           start mirror image vertically..." << endl;
    int half_Column = (width / 2);
    cout << "           half_Column...  " << half_Column << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half_Column; j++)
        {
            swap(this->r[i][j], this->r[i][width - 1 - j]);
        }
    }
    cout << "           done" << endl;
}
/**********************************************/
/**********************************************/
void Image::sepia()
{
    cout << "           start sepia filter..." << endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (0.393 * (this->r[i][j].r) + 0.769 * (this->r[i][j].g) + 0.189 * (this->r[i][j].b) > 255)
            {
                this->r[i][j].r = MAX;
            }
            else
            {
                this->r[i][j].r = 0.393 * (this->r[i][j].r) + 0.769 * (this->r[i][j].g) + 0.189 * (this->r[i][j].b);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////
            if (0.349 * (this->r[i][j].r) + 0.686 * (this->r[i][j].g) + 0.168 * (this->r[i][j].b) > 255)
            {
                this->r[i][j].g = MAX;
            }
            else
            {
                this->r[i][j].g = 0.349 * (this->r[i][j].r) + 0.686 * (this->r[i][j].g) + 0.168 * (this->r[i][j].b);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////
            if (0.272 * (this->r[i][j].r) + 0.534 * (this->r[i][j].g) + 0.131 * (this->r[i][j].b) > 255)
            {
                this->r[i][j].r = MAX;
            }
            else
            {
                this->r[i][j].b = 0.272 * (this->r[i][j].r) + 0.534 * (this->r[i][j].g) + 0.131 * (this->r[i][j].b);
            }
        }
    }
    cout << "           done" << endl;
}
/**********************************************/
/**********************************************/
void Image::strip_curtain(int curtain_W, int dis_curtain, int c_R, int c_G, int c_B)
{
    cout << "           start generating strip curtain of width " << curtain_W << " and distance " << dis_curtain << " ..." << endl;
    for (int i = 0; i < height; i++) // all the Height in specific Region
    {
        for (int x = dis_curtain; x < width; x += (curtain_W + dis_curtain))
        {
            for (int j = x; j < (curtain_W + x); j++)
            {
                this->r[i][j].r = c_R;
                this->r[i][j].g = c_G;
                this->r[i][j].b = c_R;
            }
        }
    }
    cout << "           done" << endl;
}

/**********************************************/
/**********************************************/
/****************Main Function*****************/
/**********************************************/
int main(void)
{
    int Choice;
    int i = 1;
    Image newimage;
    while (i == 1)
    {
        cout << "-------------------------------------------" << endl;
        cout << "0 Ende                / end\n"
                "1 lese .ppm Datei     / read .ppm file\n"
                "2 vertikal spiegeln   / mirror vertically\n"
                "3 schreibe .ppm Datei / write .ppm file\n"
                "4 Sepia Filter (Vergilbung) / sepia filter (yellowing)\n"
                "5 Streifenvorhang           / strip curtain\n";
        cin >> Choice;
        switch (Choice)
        {
        case 0:
            i = 0;
            break;
        case 1:
            newimage.read_ppm("peppers.ppm");
            break;
        case 2:
            newimage.mirror_vertically();
            break;
        case 3:
            newimage.write_ppm("peppers_Processed.ppm");
            break;
        case 4:
            newimage.sepia();
            break;
        case 5:
            int curtain_W;
            int dis_curtain;
            int c_R;
            int c_G;
            int c_B;
            cout << "please input width of strips: ";
            cin >> curtain_W;
            cout << "please input distance between strips: ";
            cin >> dis_curtain;
            cout << "please input R G B values: ";
            cin >> c_R >> c_G >> c_B;
            newimage.strip_curtain(curtain_W, dis_curtain, c_R, c_G, c_B);
            break;
        default:
            break;
        }
    }
    return 0;
}
