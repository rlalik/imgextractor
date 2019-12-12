#include <getopt.h>
#include <string>
#include <fstream>

#include <TCanvas.h>
#include <TFile.h>
#include <TGaxis.h>
#include <TKey.h>
#include <TROOT.h>
#include <TF1.h>

#include <TASImage.h>
#include <TString.h>

#include <TApplication.h>
#include <TStyle.h>
#include <TSystem.h>

#include <RootTools.h>

#include "functions.h"

#include <iostream>
#define PR(x) std::cout << "++DEBUG: " << #x << " = |" << x << "| (" << __FILE__ << ", " << __LINE__ << ")\n";

int flag_png;
int flag_eps;
int flag_pdf;
int flag_macro;
int flag_gencfg;
TString par_gencfg;
std::ofstream ofstr_gencfg;

int flag_width = 800;
int flag_height = 600;

std::string outpath = "./";

TDirectory * target;

FilterState global_filter = FS_None;
FilterMap global_map;

TString filter = "";

unsigned int counter = 0;

void exportimg(TObject * obj, TDirectory * dir, const CanvasCfg & ccfg)
{
    TCanvas * can = nullptr;

    dir->GetObject(obj->GetName(), can);

    can->Draw();
    can->SetCanvasSize(ccfg.w, ccfg.h);

    std::cout << "Exporting " << can->GetName() << " w=" << ccfg.w << " h=" << ccfg.h << std::endl;

    if (flag_png) RootTools::ExportPNG((TCanvas*)can, outpath);
    if (flag_eps) RootTools::ExportEPS((TCanvas*)can, outpath);
    if (flag_pdf) RootTools::ExportPDF((TCanvas*)can, outpath);
    if (flag_macro) RootTools::ExportMacroC((TCanvas*)can, outpath);
    ++counter;
}

void browseDir(TDirectory * dir, FilterState & fs, const FilterMap & filter_map)
{
    TKey * key;
    TIter nextkey(dir->GetListOfKeys());
    while ((key = (TKey*)nextkey()))
    {
        TObject *obj = key->ReadObj();

        if (obj->InheritsFrom("TDirectory"))
        {
            TDirectory * dir = (TDirectory*)obj;
            browseDir(dir, fs, filter_map);
        }
        else
        if (obj->InheritsFrom("TCanvas"))
        {
            if (flag_gencfg)
            {
                TCanvas * can = (TCanvas*)obj;
                can->Draw();
                if (ofstr_gencfg.is_open())
                    ofstr_gencfg << obj->GetName() << "\t" <<
                        "w=" << can->GetWindowWidth() << " " <<
                        "h=" << can->GetWindowHeight() << std::endl;
                continue;
            }

            CanvasCfg ccfg = { 1, flag_width, flag_height };
            FilterMap::const_iterator fit = filter_map.find(obj->GetName());
            bool found_fit = ( fit != filter_map.end() );

            if (fs == FS_Exclusive)
            {
                if (!found_fit)
                    continue;

                if (fit->second.cnt <= 0)
                    continue;

                ccfg = fit->second;
                exportimg(obj, dir, ccfg);
            }
            else if (fs == FS_Modify)
            {
                if (found_fit)
                {
                    if (fit->second.cnt < 0)
                        continue;
                    ccfg = fit->second;
                }

                exportimg(obj, dir, ccfg);
            }
            else
            {
                exportimg(obj, dir, ccfg);
            }
        }
    }
}

bool extractor(const std::string & file)
{
    TFile * f = TFile::Open(file.c_str(), "READ");

    if (!f->IsOpen())
    {
        std::cerr << "File " << file << " not open!" << std::endl;
        return false;
    }

    if (flag_gencfg)
    {
        if (par_gencfg.Length() == 0)
        par_gencfg = generate_cfg_name(file);

        ofstr_gencfg.open(par_gencfg.Data());

        f->cd();
        FilterState local_filter = FS_None;
        browseDir(f, local_filter, global_map);
        ofstr_gencfg.close();
    }
    else
    {
        std::string imgcfg_name = generate_cfg_name(file).Data();

        FilterState local_filter = global_filter;
        FilterMap local_map = global_map;

        CanvasCfg can_def = {1, flag_width, flag_height };
        if (!global_map.size())
        {
            local_filter = parser(imgcfg_name, local_map, can_def);
        }

        std::cout << "Maps summary for mode " << local_filter << std::endl;
        FilterMap & total_map = local_map;

        for (FilterMap::const_iterator it = total_map.begin(); it != total_map.end(); ++it)
        {
            if (it->second.cnt > 0)
                std::cout << " " << it->first << " [" << it->second.cnt << "] w = " << it->second.w << " h = " << it->second.h << std::endl;
            else
                std::cout << " " << it->first << " [" << it->second.cnt << "] " << std::endl;
        }

        f->cd();

        browseDir(f, local_filter, local_map);

        f->Close();
        std::cout << "Total: " << counter << std::endl;
    }
        return true;
}

int main(int argc, char ** argv)
{
    TROOT AnalysisDST_Cal1("TreeAnalysis","compiled analysisDST macros");
    TApplication app();
    gROOT->SetBatch();

    flag_gencfg = false;

    std::string text_format = ".5g";
    int max_num_digits = 3;

    struct option lopt[] =
        {
            {"png",            no_argument,        &flag_png,    1},
            {"eps",            no_argument,        &flag_eps,    1},
            {"pdf",            no_argument,        &flag_pdf,    1},
            {"C",            no_argument,        &flag_macro,1},
            {"dir",            required_argument,    0,        'd'},
            {"width",        required_argument,    0,        'w'},
            {"height",        required_argument,    0,        'h'},
            {"filter",        required_argument,    0,        'f'},
            {"gencfg",        no_argument,        0,        'g'},
            {"fmt",            required_argument,    0,        'm'},
            {"maxdig",        required_argument,    0,        'n'},
            { 0, 0, 0, 0 }
        };


    Int_t c = 0;
    while (1)
    {
        int option_index = 0;

        c = getopt_long(argc, argv, "d:w:h:f:gm:n:", lopt, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                if (lopt[option_index].flag != 0)
                    break;
                printf ("option %s", lopt[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
            case 'd':
                outpath = optarg;
                break;
            case 'w':
                flag_width = atoi(optarg);
                break;
            case 'h':
                flag_height = atoi(optarg);
                break;
            case 'f':
                global_filter = FS_Exclusive;
                {
                    CanvasCfg cc = { 99, flag_width, flag_height };
                    global_map[optarg] = cc;
                }
                break;
            case 'g':
                flag_gencfg = true;
                if (optarg)
                {
                    par_gencfg = optarg;
                }
                break;
            case 'm':
                text_format = optarg;
                break;
            case 'n':
                max_num_digits = atoi(optarg);
                break;
            case '?':
//                 Usage();
                exit(EXIT_SUCCESS);
                break;
//            case '?':
//                 abort();
//                break;
            default:
                break;
        }
    }

    gStyle->SetPaintTextFormat(text_format.c_str());
    TGaxis::SetMaxDigits(max_num_digits);

    target = gDirectory;

    RootTools::NicePalette();
    RootTools::MyMath();

    if ( ! (flag_png | flag_eps | flag_pdf | flag_macro) )
        flag_png = 1;

    if (global_map.size())
    {
        std::cout << "Filtering for :" << std::endl;
        for (FilterMap::iterator it = global_map.begin(); it != global_map.end(); ++it)
            std::cout << " " << it->first << std::endl;
    }

    while (optind < argc)
    {
        std::cout << "Extracting from " << argv[optind] << std::endl;
        extractor(argv[optind++]);
    }

    return 0;
}
