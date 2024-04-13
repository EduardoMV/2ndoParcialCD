const HTTP = require("http");
const { Router } = require("./router");
const { renderHandlebars } = require("./utils");
const port = process.env.PORT || 3000;

const router = new Router();

router.get("/", (req, res) => {
    renderHandlebars(res, 200, "./Views/home.hbs", 1);
});

const server = HTTP.createServer((req, res) => {
    router.use(req, res);
});

server.listen(port, () => {});
