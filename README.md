# OBS Squawk - Real-time Text-to-Speech Engine

The OBS Squawk plugin adds powerful voice cloning capabilities to OBS by leveraging sherpa-onnx. 
With this plugin, you can generate speech on the fly and in real-time inside OBS without any external services or access to the network.

## Features

- **OBS Audio Source**: Seamlessly integrates with OBS as an audio source.
- **Sherpa-onnx**: Utilizes [sherpa-onnx](https://github.com/k2-fsa/sherpa-onnx/) for high-quality voice synthesis and cloning.
- **Cross-Platform**: Works on any operating system.
- **Extensive Voice Library**: Access to a huge library of pre-trained voices.

## Installation

1. Download the latest release from the [Releases](https://github.com/your-repo/obs-squawk/releases) page.
2. Run the installer or extract the files from the zip to your OBS plugins directory.
3. Restart OBS to load the new plugin.

## Usage

1. Open OBS and add a new Audio Source.
1. Select the `Squawk Text to Speech` from the list of available audio sources.
1. Configure the plugin settings:
    - **Select Voice**: Choose a pre-trained voice from the library.
    - You can generate speech directly from the plugin settings
    - Set up the automated input for generation from a Text source or a file
1. Send text to the source to produce the audio by monitoring a OBS text source or a text file.

## Voice Training

Instructions on how to train a voice model will be provided soon.

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
