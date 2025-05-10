Oblivion Remastered Script Extender v0.1.0 pre-alpha
by Ian Patterson

The Oblivion Remastered Script Extender, or OBSE64 for short, is a modder's resource that expands the modding capabilities of Oblivion Remastered. It's a plugin loader for now because people are making plugins with no version compatibility detection and that's dangerous/confusing for users. Fixing some of the areas where mods would conflict with each other is planned. We'll see where modding for this game goes - many things can be done from the embedded engine side, and other things can be done from the Unreal side, but communication back and forth is a problem. As modding will probably be very different, this is not intended to be a direct port of classic OBSE's functionality.

[ Compatibility ]

OBSE64 will support the latest version of Oblivion Remastered available on Steam, currently 0.411.140.0. The Gamepass/MS Store/EGS version is unsupported. Do not email asking for support.

[ Installation ]

1. Copy the dll and exe files in to your Oblivion Remastered folder. This is usually C:\Program Files\Steam\SteamApps\common\Oblivion Remastered\OblivionRemastered\Binaries\Win64.
2. Ignore the src folder. It is there for programmers only.
3. Make sure Steam is running, then launch the game by running obse64_loader.exe.

[ FAQ ]

* Console version?
- No, not possible due to restrictions put in place by the manufacturers.

* My virus scanner complains about obse64_loader!
- It is not a virus. To extend the game, we use a technique called DLL injection to load our code. Since this technique can also be used by viruses, some badly-written virus scanners assume that any program doing it is a virus. Adding an exception to your scanner's rules may be necessary.

* Can I modify and release my own version of OBSE64 based on the included source code?
- No; the suggested method for extending OBSE64 is to write a plugin. If this does not meet your needs, please email the contact addresses listed below.

* Can I include OBSE64 as part of a mod pack/collection or otherwise rehost the files?
- No. Providing a link to https://www.nexusmods.com/oblivionremastered/mods/282 is the suggested method of distribution. Exceptions may be given under applicable circumstances; contact us at the email addresses below. This means that if you see this file available for download anywhere other than https://obse.silverlock.org, https://www.github.com/ianpatt/obse64, or on Nexus by any user other than https://www.nexusmods.com/users/2166624, that service is violating copyright. I don't like having to explicitly spell this out, but my hand has been forced.

* Where did the log files go?
- <My Documents>\My Games\Oblivion Remastered\OBSE\Logs\

* Where does the obse.ini file go?
- OBSE\obse.ini. The .ini is generally not needed. See the whatsnew for .ini options.

* How do I uninstall OBSE64?
- Delete the .dll and .exe files starting with obse64_ from your Oblivion Remastered folder.

[ Standard Disclaimer ]

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
