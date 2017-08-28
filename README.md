REQUIREMENTS

C++ Libraries:
  * Boost
  * GLM
  * SFML 2.x

Build system:
  * Ninja
  * BuildFox (https://github.com/beardsvibe/buildfox) Ninja generator, I recommend to install it
    by pip inside virtualenv



BUILD INSTRUCTIONS

Run command `bf variation=release` to generate ninja files

Run command `ninja` to build the project

The binaries will be copied to project's main directory.



USAGE INSTRUCTIONS

Not available yet, sorry. Most of the binaries receive path to one image as the commandline input
and produce some files and windows with preview as the result.
Example:
`./block_test res/photo.jpg`
