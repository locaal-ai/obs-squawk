# OBS Squawk - Real-time Versatile Text-to-Speech Source

<div align="center">

[![GitHub](https://img.shields.io/github/license/occ-ai/obs-squawk)](https://github.com/occ-ai/obs-squawl/blob/main/LICENSE)
[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/occ-ai/obs-squawk/push.yaml)](https://github.com/occ-ai/obs-squawk/actions/workflows/push.yaml)
[![Total downloads](https://img.shields.io/github/downloads/occ-ai/obs-squawk/total)](https://github.com/occ-ai/obs-squawk/releases)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/occ-ai/obs-squawk)](https://github.com/occ-ai/obs-squawk/releases)
[![Discord](https://img.shields.io/discord/1200229425141252116)](https://discord.gg/KbjGU2vvUz)

</div>

The OBS Squawk plugin adds powerful voice cloning capabilities to OBS by leveraging sherpa-onnx. 
With this plugin, you can generate speech on the fly and in real-time inside OBS without any external services or access to the network.

<div align="center">
    <a href="https://www.youtube.com/watch?v=xE41VQjTruA" target="_blank">
        <img width="40%" src="https://github.com/occ-ai/obs-squawk/assets/441170/f627fc35-df8f-4ed9-ba99-205e04c3b7c0" /><br/>
        3 Minute Tutorial
    </a>
</div>

## Features

- **OBS Audio Source**: Seamlessly integrates with OBS as an audio source.
- **Sherpa-onnx**: Utilizes [sherpa-onnx](https://github.com/k2-fsa/sherpa-onnx/) for high-quality voice synthesis and cloning. Everything built-in - not relying on any external software!
- **Cross-Platform**: Works the same way on any operating system that OBS suports: Windows, Mac and Linux.
- **Extensive Voice Library**: Access to a huge library of pre-trained voices for dozens of languages.
- **Automated Generation**: Monitor an OBS source or text file, when the content changes speech will be generated. Perfect for any automation or for live Transcription-Translation-Generation use case.
- **Real-Time & Lightweight**: The very efficient VITS architecture for speech generation is extremel resource efficient and runs real-time on a modest CPU.

## Installation

1. Download the latest release from the [Releases](https://github.com/occ-ai/obs-squawk/releases) page.
2. Run the installer or extract the files from the zip to your OBS plugins directory.
3. Restart OBS to load the new plugin.

## Usage

1. Open OBS and add a new Audio Source.
1. Select the `Squawk Text to Speech` from the list of available audio sources.
1. Configure the source settings:
    - Select Voice: Choose a pre-trained voice package from the library. Models will be downloaded as necessary.
    - Select the Speaker ID: some voice packages have multiple (even 100s) speakers.
    - You can generate speech directly from the plugin settings by clicking the button.
    - Set up the monitoring of a Text source or a file.
1. Send text to the source to produce the audio by monitoring a OBS text source or a text file.

### Voice Training

Instructions on how to train a voice model to clone your own or otheres voice will be provided soon!

### Models

The plugin lists the [directory of models](https://github.com/k2-fsa/sherpa-onnx/releases/tag/tts-models) offered by sherpa-onnx.

Using an external model for generation will be available soon!

## Building

### Mac OSX

Using the CI pipeline scripts, locally you would just call the zsh script:

```sh
$ ./.github/scripts/build-macos -c Release
```

#### Install
The above script should succeed and the plugin files (e.g. `obs-squawk.plugin`) will reside in the `./release/Release` folder off of the root. Copy the `.plugin` file to the OBS directory e.g. `~/Library/Application Support/obs-studio/plugins`.

To get `.pkg` installer file, run for example
```sh
$ ./.github/scripts/package-macos -c Release
```
(Note that maybe the outputs will be in the `Release` folder and not the `install` folder like `pakage-macos` expects, so you will need to rename the folder from `build_x86_64/Release` to `build_x86_64/install`)

### Linux

#### Ubuntu

For successfully building on Ubuntu, first clone the repo, then from the repo directory:
```sh
$ sudo apt install -y libssl-dev libarchive-dev libbzip2-dev
$ ./.github/scripts/build-linux
```

Copy the results to the standard OBS folders on Ubuntu
```sh
$ sudo cp -R release/RelWithDebInfo/lib/* /usr/lib/
$ sudo cp -R release/RelWithDebInfo/share/* /usr/share/
```
Note: The official [OBS plugins guide](https://obsproject.com/kb/plugins-guide) recommends adding plugins to the `~/.config/obs-studio/plugins` folder. This has to do with the way you *installed* OBS.

In case the above doesn't work, attempt to copy the files to the `~/.config` folder:
```sh
$ mkdir -p ~/.config/obs-studio/plugins/obs-squawk/bin/64bit
$ cp -R release/RelWithDebInfo/lib/x86_64-linux-gnu/obs-plugins/* ~/.config/obs-studio/plugins/obs-squawk/bin/64bit/
$ mkdir -p ~/.config/obs-studio/plugins/obs-squawk/data
$ cp -R release/RelWithDebInfo/share/obs/obs-plugins/obs-squawk/* ~/.config/obs-studio/plugins/obs-squawk/data/
```

#### Other distros

For other distros where you can't use the CI build script, you can build the plugin as follows

1. Clone the repository and install these dependencies using your distribution's package manager:

    * libssl (with development headers)

2. Generate the CMake build scripts (adjust folders if necessary)

    ```sh
    cmake -B build-dir --preset linux-x86_64 -DUSE_SYSTEM_CURL=ON -DCMAKE_INSTALL_PREFIX=./output_dir
    ```

3. Build the plugin and copy the files to the output directory

    ```sh
    cmake --build build-dir --target install
    ```

4. Copy plugin to OBS plugins folder

    ```sh
    mkdir -p ~/.config/obs-studio/plugins/bin/64bit
    cp -R ./output_dir/lib/obs-plugins/* ~/.config/obs-studio/plugins/bin/64bit/
    ```

    > N.B. Depending on your system, the plugin might be in `./output_dir/lib64/obs-plugins` instead.

5. Copy plugin data to OBS plugins folder - Possibly only needed on first install

    ```sh
    mkdir -p ~/.config/obs-studio/plugins/data
    cp -R ./output_dir/share/obs/obs-plugins/obs-squawk/* ~/.config/obs-studio/plugins/data/
    ```

### Windows

Use the CI scripts again, for example:

```powershell
> .github/scripts/Build-Windows.ps1 -Configuration Release
```

The build should exist in the `./release` folder off the root. You can manually install the files in the OBS directory.

```powershell
> Copy-Item -Recurse -Force "release\Release\*" -Destination "C:\Program Files\obs-studio\"
```

## Contributing

We welcome contributions! Please fork the repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the GPLv2 License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [sherpa-onnx](https://github.com/k2-fsa/sherpa-onnx) for providing the underlying voice synthesis technology and pretrained models.

## Support

For support and troubleshooting, please visit our [GitHub Issues](https://github.com/occ-ai/obs-squawk/issues) page or reach out on [our Discord server](https://discord.gg/s5K9RwPV7w).

---

Thank you for using the OBS Squawk Plugin! Enhance your streams with the power of voice cloning.
