# DeskSort 📁
A lightweight and efficient desktop application that helps you keep your workspace organized by automatically sorting and moving files into folders based on customizable user-defined rules.

### Warning ⚠️
This was made for learning purposes, although it might work, there might be huge bugs so use it at your own risk.

There are currently no safeguards against setting critical system folders (e.g., C:\Windows\System32) as sources.

## What it does
It runs on the background, and scans every file in every source directory set. If it matches a pattern you set (File extension, name prefix, suffix, containing) then it will move it to the destination folder. **It will skip folders and symbolic links**.

It repeats this for every minute interval you set, and it starts on startup.

## Features
- **Rule based sorting** - You can set the rules, what type of files should be moved
- **Custom folder mapping** - Set your own source and destination folders
- **Runs in background** - It will keep working silently without interrupting your workflow
- **Auto-Start** – Automatically launches when your system boots.
- **Minimize to system tray** - Keeps the app out of sight but easily accessible from the tray.

## Built with
- ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
- ![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
- ![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)
- ![SQLite](https://img.shields.io/badge/sqlite-%2307405e.svg?style=for-the-badge&logo=sqlite&logoColor=white)

## Future plans
- Fix UI elements, make it more clear for each action the user does (Selecting, etc)
- Optimize code maybe
- Cross platform maybe
- Prevent users from adding dangerous system folders as source paths.
- Properly name and set icons for windows other than the main window

## Downloads
Get the latest build from the [GitHub Releases](https://github.com/osten-antonio/DeskSort/releases) page.

## Demo

Uploading demovid (1).mp4…


## Contributing

Contributions are always welcome!

Feel free to open issues, suggest new features, or submit a pull request.
