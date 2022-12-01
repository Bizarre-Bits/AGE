# AGE: Another Game Engine
<div align="center"><img alt="AGE logo" src="./age_logo_text.svg" height="500"/></div>

<div align="center">
<p>
  <img alt="Supported platforms" src="https://img.shields.io/static/v1?label=Platforms&message=Linux&color=1aa943&style=flat-square">
  <img alt="Version" src="https://img.shields.io/static/v1?label=Version&message=0.0.0pre&color=red&style=flat-square">
</p>
<p><img alt="Licence" src="https://img.shields.io/static/v1?label=Licence&message=Apache%202.0&color=1aa943&style=flat-square"></p>
  <p>
    <img alt="C++ version" src="https://img.shields.io/static/v1?label=C%2b%2b&message=23&color=00599C&style=flat-square">
    <img alt="CMake version" src="https://img.shields.io/static/v1?label=CMake&message=3.7&color=064F8C&style=flat-square">
    <img alt="OpenGL version" src="https://img.shields.io/static/v1?label=OpenGL&message=4.6&color=5586A4&style=flat-square">
  </p>
</div>

A game engine written in C++ just for fun and education. 
With it I'm learning concepts and practces underlying the creation of a game engine and some concepts related to github.

and for some info on project's future and some roadmap you can check [AGEngine project](https://github.com/users/nightingazer/projects/1)

Highly inspired by [Yan Chernikov's](https://github.com/TheCherno) [Hazel Engine](https://github.com/TheCherno/Hazel), thanks to him for his wonderfull series on the matter.

## Startup guide

To clone this repo with submodules run

``` bash
git clone --recurse-submodules https://github.com/nightingazer/AGE.git
```

### Ubuntu

By default GLFW is builded with x11, so in that case you should have xorg-dev installed on your system in order to build the AGE.
To install it run
```bash
sudo apt-get install xorg-dev
```
If you want to run the Sandbox app with Nvidia dGPU you should add `__NV_PRIME_RENDER_OFFLOAD=1` and `__GLX_VENDOR_LIBRARY_NAME=nvidia` to the environment variables of your run. (At least it works on my laptop with Intel iGPU and Nvidia dGPU)
