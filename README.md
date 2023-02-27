# PHPBrowserBox


Table of contents:
* [Introduction](#introduction)
* [Installation](#installation)
* [Downloads](#downloads)
* [Support](#support)
* [Seeking sponsors](#sponsors)


## Introduction


PHPBrowserBox is an open source project founded by Anthony Ogundipe
in 2013 to provide a way for developing native desktop GUI applications
using web technologies such as PHP, HTML5, JavaScript and SQLite.
It was originally built with visual basic using IE as internal explorer, but over time it has evolved to include Apache, MySQL, SQLite, and even use google chrome internally and have support for NODE.JS module.

![3 ](https://user-images.githubusercontent.com/948100/221455215-802497e3-051d-415f-b109-4e3ed24b1bae.png)

Earlier versions were built with CPP, even NODE Webkit, but this latest verson as 2023 release is built on Electron JS and it actually allows you to package just HTML app alone, Apache/PHP is optional, MySQL is optional. These configurations are made so to allow for flexible.

![php-8 2](https://user-images.githubusercontent.com/948100/221455117-641db17a-557f-4319-8b9e-cd77da7ead03.png)

It is a convienient tool for converting PHP web apps and PHP CLI tools to desktop applications with little effort.

The development workflow you are used to while creating web applications
remains the same, there is no new framework / API to learn. The process
of turning an existing website into a desktop application is basically
a matter of copying it to the "phpbrowserbox/app/htdocs/" directory.

In a certain sense phpbrowserbox acts differently from a PHP to EXE compiler. It embeds
a web browser, a multi-threaded web server and a PHP interpreter. All
embedded into a single application, a portable folder that you can easily
distribute to end users by packing it to zip archive or by making an
[installer for your application](../../wiki/Knowledge-Base#application-installer).

The web server embedded is a apache, mysql, php, and google chrome via  
[ElectronJS](https://electronjs.org).

Technically all php versions are supported, but official support is php 7.3 and above.
All popular PHP frameworks are supported, see the
[PHP frameworks support](../../wiki/PHP-frameworks-support) wiki page
for example configurations for CakePHP, CodeIgniter, Laravel, Symfony, Yii and Zend Framework.  


You can create a standalone executable for
distribution with the help of the
[Inno Setup installer](../../wiki/Knowledge-Base#application-installer).
PHP sources can be protected with the many of the available
[PHP encoders](../../wiki/Knowledge-Base#how-do-i-protect-php-sources-in-the-www-directory).

PHPBrowserBox is released under non-restrictive license, thus it is
[free for commercial use](../../wiki/Knowledge-Base#can-i-use-php-browserbox-in-a-commercial-closed-sourced-project).


Lots of other useful information can be found on the
[Knowledge Base](../../wiki/Knowledge-Base) wiki page and on the
[PHPBrowserBox Website](https://phpbrowserbox.com).

## Installation

#### Installation Requirements
While older versions of phpbrowserbox (v4.0 and below) supports were mainly 32 bits but may work in 64 bits windows.
The new version phpvrowserbox v5.0 was developed and tested on Windows 10, 64Bit but should work in some 32 bits PCs (this has not been tested at this time).
Also the previous versions have everything bundled together, the most recent version splits the application into two : the phpbrowserbox app itself which by default will not run php until you download the appropriate support package.

#### Installation Steps

* Step 1: Download PHPBrowserBox Application:
Download using the links in the download section and ensure you can run it from your system by opening the folder and double-clicking on the phpbrowserbox application icon.

* Step 2: Download the appropriate binary package:
The support packages are based on the php version your application require to work. If your application require php 7.4 for example, download the "php 7.4 support package", extract the archive on your pc, copy the bin folder and replace the one inside the application you downloaded in step 1. The location of the bin folder is "phpbrowserbox/app/bin"

* Step 3: Configure the application.json in phpbrowserbox/app/main
This specify the behaviour of PHPBrowserBox and determines how the application launches, whether with startup screen or not, what page to load as startup e.t.c

* Step 4: Setup your startup screen/html app
When PHPBrowserBox starts up, it may take some few seconds for the servers to start in the background and you may want to display a splash screen page at the beginning, this can be configured in application.json. The startup page by default is located in **"phpbrowserbox/app/startup"** and it should have an index.html though this can be changed in the configuration

* Step 5: Setup your php application
Finally, we are here. Your php application should be copied inside "phpbrowserbox/app/htdocs". I should mention straight that .htaccess is supported out of the box (unlike previous versions)

    - You can find out more from  the [PHPBrowserBox v5.0.0 Installation Guide](https://github.com/dhtml/phpbrowserbox/wiki/PHP-BrowserBox-v5.0.0-installation-guide)


## Downloads

  * PHPBrowserBox Application
    - [PHPBrowserBox v5.0.0 for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/version-v5.0-rc)
      release
    - [PHPBrowserBox v5.0.0 Guide](https://github.com/dhtml/phpbrowserbox/wiki/PHP-BrowserBox-v5.0.0)
      wiki page

  * PHPBrowserBox Support Packages
    - [phpv7.0 support package for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/php-version-v7.0-rc)
      release
    - [phpv7.3 support package for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/php-version-v7.3-rc)
      release
    - [phpv7.4 support package for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/php-version-v7.4-rc)
      release
    - [phpv8.0 support package for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/php-version-v8.0-rc)
      release
    - [phpv8.1 support package for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/php-version-v8.1-rc)
      release
    - [phpv8.2 support package for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/php-version-v8.2-rc)
      release
    - [PHPBrowserBox Support Packages](https://github.com/dhtml/phpbrowserbox/wiki/PHP-BrowserBox-support-v5.0.0)
      wiki page

  * PHPBrowserBox Archives
    - [PHPBrowserBox v1.0.0 for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/version-v1.0-rc)
      release
    - [PHPBrowserBox v2.0.0 for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/version-v2.0-rc)
      release
    - [PHPBrowserBox v3.0.0 for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/version-v3.0-rc)
      release
    - [PHPBrowserBox v4.0.0 for Windows
](https://github.com/dhtml/phpbrowserbox/releases/tag/version-v4.0-rc)
      release

  * Linux
    - No release yet

  * Mac
    - No release yet


## Support

* Documentation is on the [Wiki Pages](../../wiki). Start with the
  [Knowledge Base](../../wiki/Knowledge-Base), [Settings](../../wiki/Settings)
  and [Chrome settings](../../wiki/Chrome-settings) wiki pages.
* Ask questions and report problems on the
  [PHPBrowserBox Website](https://phpbrowserbox.com)



## Sponsors

PHPBrowserBox is seeking companies to sponsor further development of the project.

If your company would like to sponsor PHPBrowserBox development efforts
then please contact [Anthony](https://www.linkedin.com/in/anthonyogundipe/).

Long term sponsorships are welcome and Anthony is open to ideas about
the project. He would love to spend more time on developing this project,
but he can't afford doing so in his free time.
