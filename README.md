# OBS Squawk - Real-time Text-to-Speech Engine

The OBS Squawk plugin adds powerful voice cloning capabilities to OBS by leveraging sherpa-onnx. With this plugin, you can enhance your audio sources with a huge library of voices and even train new voice models for unique voice cloning.

## Features

- **OBS Audio Source**: Seamlessly integrates with OBS as an audio source.
- **Sherpa-onnx**: Utilizes sherpa-onnx for high-quality voice synthesis and cloning.
- **Cross-Platform**: Works on any operating system.
- **Extensive Voice Library**: Access to a huge library of pre-trained voices.
- **Custom Voice Training**: Ability to train new voice models for personalized voice cloning.

## Installation

1. Download the latest release from the [Releases](https://github.com/your-repo/obs-squawk/releases) page.
2. Extract the files to your OBS plugins directory.
3. Restart OBS to load the new plugin.

## Usage

1. Open OBS and add a new Audio Source.
1. Select the `OBS Squawk Plugin` from the list of available audio sources.
1. Configure the plugin settings:
    - **Select Voice**: Choose a pre-trained voice from the library.
1. Send text to the source to produce the audio by monitoring a OBS text source or a text file.

## Voice Training

To train a new voice model:
1. Gather audio samples of the voice you want to clone.
2. Use the sherpa training interface to upload and process the samples.
3. Once training is complete, the new voice will be available in the voice selection menu.

## Contributing

We welcome contributions! Please fork the repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [sherpa-onnx](https://github.com/k2-fsa/sherpa-onnx) for providing the underlying voice synthesis technology.

## Support

For support and troubleshooting, please visit our [GitHub Issues](https://github.com/occ-ai/obs-squawk/issues) page.

---

Thank you for using the OBS Squawk Plugin! Enhance your streams with the power of voice cloning.
