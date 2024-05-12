#include <nana/gui.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <raudio2/raudio2.hpp>
#include <string>
#include <vector>

static const std::string mainDiv{ R"(
<>
  <weight=80% vertical
    <>
    <vertical gap=0 info arrange=[24,24]>
    <weight=24 gap=10 buttons>
    <>
  >
<>)" };

int main()
{
    ra::AudioDevice audioDevice;
    ra::Music song;
    std::string songTitle{ "-" };

    if (!audioDevice.Init())
    {
        nana::msgbox msgBox(nullptr, "nana player");
        msgBox << "Failed to load raudio2.";
        msgBox
            .icon(nana::msgbox::icon_error)
            .show();
        return -1;
    }

    nana::appearance frmMainAppearance(true, true, false, false, true, false, false);
    nana::form frmMain(nana::API::make_center(320, 160), frmMainAppearance);
    nana::place plcMain{ frmMain };
    nana::label lblStatus{ frmMain };
    nana::label lblTitle{ frmMain };
    nana::button btnPlay{ frmMain };
    nana::button btnStop{ frmMain };
    nana::button btnOpen{ frmMain };
    nana::filebox fbOpenSong{ frmMain, true };

    auto openFile = [&](std::vector<std::filesystem::path> files) {
        if (files.size() >= 1)
        {
            auto filePath = files[0].generic_u8string();
            auto [music, success] = audioDevice.LoadMusic(filePath.c_str(), false);
            if (success)
            {
                song = std::move(music);
                auto artist = song.getArtist();
                auto title = song.getTitle();
                if (!title.empty())
                    if (!artist.empty())
                        songTitle = std::string(artist) + std::string(" - ") + std::string(title);
                    else
                        songTitle = title;
                else
                    songTitle = files[0].filename().filename().generic_u8string();

                song.Play();
                lblStatus.caption("Playing");
                lblTitle.caption(songTitle);
            }
        }
    };

    auto generateFileFilters = [&](const char** namesPtr) -> std::vector<std::pair<std::string, std::string>> {
        std::vector<std::pair<std::string, std::string>> fileFilters;
        for (; *namesPtr; namesPtr++)
        {
            std::string pluginName(*namesPtr);
            auto pluginDescription = pluginName + " files";
            auto pluginExtensions = std::string("input.") + pluginName + ".plugin_extensions";

            auto ExtensionsPtr = audioDevice.getStringCharArray(pluginExtensions);
            std::string filter;
            for (; *ExtensionsPtr; ExtensionsPtr++)
            {
                if (!filter.empty())
                {
                    filter += ";";
                }

                filter += "*" + std::string(*ExtensionsPtr);
            }
            fileFilters.push_back(std::make_pair(pluginDescription, filter));
        }
        return fileFilters;
    };

    auto inputFilters = generateFileFilters(audioDevice.getInputPluginNames());
    auto archiveFilters = generateFileFilters(audioDevice.getArchivePluginNames());

    std::string globalFilter;
    for (const auto& filter : inputFilters)
    {
        if (!globalFilter.empty())
        {
            globalFilter += ";";
        }
        globalFilter += filter.second;
    }

    fbOpenSong.add_filter("All raudio2 files", globalFilter);
    fbOpenSong.add_filter(inputFilters);
    fbOpenSong.add_filter(archiveFilters);

    frmMain.caption("nana player");
    frmMain.enable_dropfiles(true);

    frmMain
        .events()
        .mouse_dropfiles([&](const nana::arg_dropfiles& arg) {
            openFile(arg.files);
        });

    lblStatus
        .caption("Open a file.")
        .tooltip("Status");

    lblTitle
        .caption(songTitle)
        .tooltip("Song Title");

    btnPlay
        .caption("Play/Pause")
        .tooltip("Play/Pause")
        .events().click([&]() {
            if (!song.IsPlaying())
            {
                song.Play();
                lblStatus.caption("Playing");
            }
            else
            {
                song.Pause();
                lblStatus.caption("Paused");
            }
        });

    btnStop
        .caption("Stop")
        .tooltip("Stop")
        .events()
        .click([&]() {
            song.Stop();
            lblStatus.caption("Stopped");
        });

    btnOpen
        .caption("Open")
        .tooltip("Open")
        .events()
        .click([&]() {
            openFile(fbOpenSong.show());
        });

    plcMain.div(mainDiv);
    plcMain.field("info") << lblStatus << lblTitle;
    plcMain.field("buttons") << btnPlay << btnStop << btnOpen;
    plcMain.collocate();

    frmMain.show();
    nana::exec();
    return 0;
}
