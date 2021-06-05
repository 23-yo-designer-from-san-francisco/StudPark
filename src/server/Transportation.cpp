#include "Transportation.h"

void Transportation::userTransportation(Request request, Response& response) {
    response.setStatus(status::NotFound);
    std::unordered_map<std::string, std::string> data;
    data = request.getData();
    data["session"] = request.cookie("session");
    if (data["session"].empty() && request.path() != "/login" && request.path() != "/signup" && request.method() == "GET") {
        response.setStatus(302);
        response.setHeader("Location", "/login");
        return;
    }

    if (request.method() == "GET" && request.path() == "/") {
        response.setStatus(302);
        if (!request.cookie("session").empty()) {
            std::cout << "Redirecting to feed" << std::endl;
            response.setHeader("Location", "/feed");
        } else {
            response.setHeader("Location", "/login");
        }
        return;
    }

    if (request.method() == "GET") {
        if (request.path() == "/UserAPI/Get") {
        } else if (request.path() == "/login") {
            response = Response(userApi.Login());
        } else if (request.path() == "/feed") {
            response = Response(postApi.Feed(data));
        } else if (request.path() == "/FilesAPI/Get") {
        } else if (request.path() == "/settings") {
            response = Response(userApi.SettingsPage(data));
        } else if (request.path() == "/profile") {
            response = Response(userApi.Profile(data));
        } else if (request.path() == "/FilesAPI/List") {
        } else if (request.path() == "/GroupAPI/Get") {
        } else if (request.path() == "/GroupAPI/ExportList") {
            response = Response(groupApi.ExportGroupList(data));
        } else if (request.path() == "/PostAPI/Get") {
        } else if (request.path() == "/userAPI/Get") {
        } else if (request.path() == "/logout") {
            std::string result = userApi.Logout(data);
            response = Response(result);
            if (result.empty()) {
                response.setStatus(302);
                response.setHeader("Location", "/");
            }
        } else if (request.path() == "/GroupAPI/Create") {
            response = Response(groupApi.CreatePage(data));
        } else if (request.path() == "/signup") {
            response = Response(userApi.SignupPage());
        } else if (request.path() == "/GroupAPI/GetDeadlines") {
            response = Response(groupApi.GetDeadlines(data));
        } else if (request.path() == "/GroupAPI/GetSubjects") {
            response = Response(groupApi.GetSubjects(data));
        } else if (request.path() == "/GroupAPI/SubjectList") {
            response = Response(groupApi.GetSubjectsPage(data));
        } else if (request.path() == "/PostAPI/Create") {
            response = Response(postApi.CreatePostPage(data));
        } else if (request.path() == "/GroupAPI/List") {
            response = Response(groupApi.Get(data));
        } else if (request.path() == "/GroupAPI/AddSubject") {
            response = Response(groupApi.AddSubjectPage(data));
        }
    } else if (request.method() == "POST") {
        std::cout << "POST";
        if (request.path() == "/") {
        } else if (request.path() == "/FilesAPI/Create") {
        } else if (request.path() == "/FilesAPI/Update") {
        } else if (request.path() == "/FilesAPI/Delete") {
        } else if (request.path() == "/FilesAPI/Share") {
        } else if (request.path() == "/GroupAPI/Create") {
            response = Response(groupApi.Create(data));
        } else if (request.path() == "/GroupAPI/Update") {
        } else if (request.path() == "/settings") {
            std::string result = userApi.Update(data);
            response = Response(result);
            if (result.empty()) {
                response.redirect("/settings");
            }
        } else if (request.path() == "/GroupAPI/Delete") {
        } else if (request.path() == "/GroupAPI/DeleteSubject") {
            std::string result = groupApi.DeleteSubject(data);
            response = Response(result);
            if (result.empty()) {
                response.redirect("/GroupAPI/SubjectList");
            } else {
                response.setStatus(500);
            }
        } else if (request.path() == "/GroupAPI/DeleteDeadline") {
            response = Response(groupApi.DeleteDeadline(data));
        } else if (request.path() == "/GroupAPI/AddSubject") {
            response = Response(groupApi.AddSubject(data));
            response.redirect("/GroupAPI/SubjectList");
        } else if (request.path() == "/GroupAPI/AddDeadline") {
            response = Response(groupApi.AddDeadline(data));
        } else if (request.path() == "/PostAPI/Create") {
            std::string result = postApi.Create(data);
                response = Response(result);
            if (result.empty()) {
                response.setStatus(302);
                response.setHeader("Location", "/feed");
            }
        } else if (request.path() == "/PostAPI/Update") {
        } else if (request.path() == "/PostAPI/Delete") {
            response = Response(postApi.Delete(data));
        } else if (request.path() == "/signup") {
            std::string result = userApi.Create(data);
            if (result.empty()) {
                response.setStatus(302);
                response.setHeader("Location", "/login");
            } else {
                response = Response(result);
            }
        } else if (request.path() == "/UserAPI/Update") {
            response = Response(userApi.Update(data));
            response.redirect("/profile");
        } else if (request.path() == "/UserAPI/Delete") {
        } else if (request.path() == "/UserAPI/Authenticate") {
            std::pair<std::string, std::string> auth = userApi.Authenticate(data);
            response = Response(auth.second);
            if (auth.second.empty()) {
                response.setStatus(302);
                response.setHeader("Location", "/feed");
                response.setCookie("session", auth.first);
            }
        } else if (request.path() == "/UserAPI/AddContact") {
        }
    }
}
