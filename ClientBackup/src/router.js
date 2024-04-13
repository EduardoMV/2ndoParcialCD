const url = require("node:url");
const { renderHandlebars } = require("./utils");

class Router {
    routes = {};
    constructor() {}

    get(path, callback) {
        this.routes[path] = { ...this.routes[path], GET: { callback } };
    }

    post(path, callback) {
        this.routes[path] = { ...this.routes[path], POST: { callback } };
    }
    /**
     * Sends an HTML using
     * @param {Request} request HTTP request
     * @param {Response} response HTTP response
     */
    use(request, response) {
        const path = url.parse(request.url).pathname;

        const route = this.routes[path];

        if (!route || !route[request.method]) {
            renderHandlebars(response, 404, "./Views/Error404.hbs");
            return;
        } else {
            route[request.method].callback(request, response);
        }
    }
}

module.exports = { Router };
