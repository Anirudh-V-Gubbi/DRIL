<div align="center">
  <h1 align="center">DRIL</h1>
</div>
<br/>

  <h3 align="center">A Dynamic Reloadable Interface for Libraries</h3>


## Features

- **Hot Reload :** Changes made to the library during runtime are visible
- **Platform Independent :** Works on all platforms Windows, Linux and MacOS
- **Inbuilt Parser :** Reads symbols from library and reloads them during runtime
- **Automatic Build :** The initial build is automatic and performed when the Premake script is run

  ## Use Cases for our Interface

- [An application using OpenGL Graphics]( https://github.com/Anirudh-V-Gubbi/DRIL/tree/main/examples/OpenGL)
    - Visible changes are made and graphics are re-rendered during runtime with the help of our Interface
 
- [A Json Parser Application](https://github.com/Anirudh-V-Gubbi/DRIL/tree/main/examples/FileParser)
    - Files to be parsed can be modified or replaced during runtime with the help of our Interface
  


## The Role of the Interface within the Build System.
 

![](https://github.com/Anirudh-V-Gubbi/DRIL/blob/main/images/Screenshot%20from%202024-06-22%2019-31-58.png)  


## Tech Stack

- [OpenGL](https://www.opengl.org/) – Use Case
- [C++](https://en.cppreference.com/w/) – Language
- [Premake](https://premake.github.io/) – Build Automation


## Inspiration and References

- [Interface Related Ideas](https://howistart.org/posts/cpp/1/index.html) 


