const Handlebars = require("handlebars");
const fs = require("node:fs");
const path = require("node:path");

const layoutPath = path.join(__dirname, "Views", "Layout.hbs");
fs.readFile(layoutPath, (err, data) => {
    if (err) throw new Error(`Error while reading ${layoutPath}`);
    else {
        const content = data.toString();
        Handlebars.registerPartial("layout", content);
    }
});

function renderHandlebars(res, status, dir, data) {
    const file = path.join(__dirname, dir);

    fs.readFile(file, (err, fileData) => {
        if (err) {
            res.writeHead(500, { "Content-Type": "text/HTML" });
            res.write("<h1>Error 500 - server could render view </h1>");
            res.end();
            return;
        } else {
            const content = fileData.toString();
            const template = Handlebars.compile(content);

            res.writeHead(status, { "Content-Type": "text/HTML" });
            res.write(template(data));
            res.end();
            return;
        }
    });
}

module.exports = { renderHandlebars };
