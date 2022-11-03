module("luci.controller.watson_controller", package.seeall)

function index()
	entry({"admin", "services", "watson"}, cbi("watson_model"), "IoT Watson cloud daemon",100)
end