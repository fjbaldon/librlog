# Project Title

LibrLog

## Description

LibrLog is a command-line tool for logging books in a library. With LibrLog, you can easily keep track of the books the institution owns, the books patrons read, and the books that are available.

Key features of LibrLog include:

- Simple and intuitive command-line interface for logging books and managing your library.
- Easy-to-use book logging commands for adding new books to your library, keeping track of available books, and updating book information.
- Search and filtering options for finding books in your library based on author, title, genre, or other criteria.
- Export options for exporting your library data to a variety of file formats, including CSV, JSON(?), and XML(?).

To get started with LibrLog, simply download the source code from our GitHub repository at [https://github.com/fjbaldon/librlog.git](https://github.com/fjbaldon/librlog.git) and follow the installation instructions in the README. Once installed, you can start logging books in your library right away using the simple and intuitive command-lineinterface.

We welcome contributions from developers of all skill levels and backgrounds, so if you're interested in contributing to the project, please visit our GitHub repository and check out our `CONTRIBUTING.md` file for guidelines and instructions on how to contribute.

Thank you for considering LibrLog for your library needs!

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [Credits](#credits)
- [License](#license)

## Installation

To install the `librlog` program, follow these steps:

1. Clone the repository to your local machine:
   ```
   git clone https://github.com/fjbaldon/librlog.git
   ```
2. Navigate to the project directory:
   ```
   cd librlog
   ```
3. Compile the program by running the following command:
   ```
   make
   ```
4. Run the program by running the following command:
   ```
   make run
   ```
5. (Optional) To remove the compiled binary file, run the following command:
   ```
   make clean
   ```
Once the `librlog` program is installed, you can run it from the command line by typing:

```
$ librlog
```

This will launch the program and allow you to log books in a library from the command line. For more information on how to use the program, see the program's documentation or user manual.

If you're using an Android device, you can install Termux from either [F-Droid](https://f-droid.org/en/packages/com.termux/) or [Google Play](https://play.google.com/store/apps/details?id=com.termux&pli=1). Once installed, you can run the following commands to ensure that your system is up-to-date and to install the necessary dependencies:

1. Update your system:
   ```
   pkg upgrade
   ```
2. Install the required dependencies:
   ```
   pkg install git clang make
   ```
3. Follow the installation instructions provided above.
 
By following these steps, you should be able to set up your Android device to compile and run C code using Termux.

## Usage

### `library_catalog.csv` Column Documentation

The `library_catalog.csv` file contains the following columns:

- `Title`: The title of the book.
- `Author`: The author of the book.
- `Publisher`: The publisher of the book.
- `Publication Year`: The year the book was published.
- `ISBN`: The ISBN (International Standard Book Number) of the book.
- `Accession Number`: A unique identifier for the book used to track its location and availability within the library.
- `Genre`: The genre of the book (e.g. fiction, non-fiction, mystery, romance, etc.).
- `Checked Out By`: The name of the patron who has checked out the book.
- `Checked Out Date`: The date the book was checked out by the patron, formatted as YYYY-MM-DD.
- `Return Date`: The date the book is due to be returned by the patron, formatted as YYYY-MM-DD.

**Note: This section is currently under development and will be updated soon. Thank you for your patience!**

## Contributing

Thank you for your interest in contributing to our project! We welcome contributions from developers of all skill levels and backgrounds.

### How to contribute

To contribute to the project, please follow these steps:

1. Check the project's issue tracker for open issues or feature requests that need attention. If you find an issue or request that you would like to work on, please comment on the issue to let us know.
2. Fork the project's repository and create a new branch foryour changes.
3. Make your changes on the branch and test them thoroughly.
4. Submit a pull request to the main repository and describe your changes in detail in the pull request description.
5. Wait for feedback from the project maintainers. We will review your changes and provide feedback or request changes if necessary.

### Development environment

To set up a development environment for the project, please follow these steps:

1. Clone the project's repository to your local machine.
2. Install any dependencies required by the project.
3. Run the project locally and test it thoroughly.

### Coding style and standards

When contributing code to the project, please follow these guidelines:

- Use consistent indentation and formatting.
- Write clear and concise comments to explain your code and changes.
- Document your code changes in the pull request description.
- Write tests for your code changes and ensure that all tests pass.

### Open issues

Here are some open issues and feature requests that need attention:

- Issue #1: Description of issue or feature request
- Issue #2: Description of issue or feature request
- ...

If you would like to contribute to any of these issues, please comment on the issue to let us know. We appreciate your help in improving the project!

## Credits

- Francis John Baldon @fjbaldon : Lead Developer
- Jaycho Carido @mojaycbug      : Contributer
- Carl James Lopez              : Contributer
- Kyte Aleszandrah Orion        : Contributer
- Julie Mae Catigum             : Contributer
- Regine Ilka Ofamen            : Contributer
- Reign Alger Ofamen            : Contributer

Finally, we would like to thank our users and anyone else who has supported the project in any way. We appreciate your feedback and contributions, and we will continue to work hard to make this project the best it can be.

## License

This project is licensed under the GNU General Public License v3.0 (GPLv3). The GPLv3 is a copyleft license that allows others to use, modify, and distribute your work, but also requires that any derivative works be licensed under the same terms.

You can read the full text of the GPLv3 at [https://www.gnu.org/licenses/gpl-3.0.en.html](https://www.gnu.org/licenses/gpl-3.0.en.html).

