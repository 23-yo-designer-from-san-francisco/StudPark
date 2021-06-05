#include "PostAPI.h"

TemplateEngine templates;

std::string PostAPI::Create(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    if (admin.role == Student::Roles::STUDENT) {
        return templates.RenderErrors("Недостаточно привилегий", 1, admin.FullName(), admin.role);
    }
    try {
        Post::AddPost(db,
                      data.find("header")->second,
                      data.find("body")->second,
                      admin.id,
                      time(nullptr));
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what(), 1, admin.FullName(), admin.role);  // Render template
    }
    return "";  // Render template
}

std::string PostAPI::CreatePostPage(const std::unordered_map<std::string, std::string> &data) {
    Student admin;
    try {
        admin = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }
    std::cout << (admin.role != Student::Roles::ADMIN) << std::endl;
    if (admin.role == Student::Roles::STUDENT) {
        return templates.RenderErrors("Недостаточно привилегий", 1, admin.FullName(), admin.role);
    }

    return templates.RenderCreatePostPage(1, admin.FullName(), admin.role);
}

std::string PostAPI::Get(const std::unordered_map<std::string, std::string> &data) {
    return "";  // Single post page, not implemented
}
std::string PostAPI::Update(const std::unordered_map<std::string, std::string> &data) {
    return "";  // Not implemented
}
std::string PostAPI::Delete(const std::unordered_map<std::string, std::string> &data) {
    Student st = Student::GetStudentBySession(db, data.find("session")->second);
    Post post = Post::GetPost(db, std::stoi(data.find("id")->second));
    if (post.owner != st.id) {
        return "Not enough privileges";
    }
    try {
        Post::DeletePost(db, std::stoi(data.find("id")->second));
    } catch (std::exception &e) {
        return e.what();
    }
    return "";
}

std::string PostAPI::AddPostComment(const std::unordered_map<std::string, std::string> &data) {
    Student st = Student::GetStudentBySession(db, data.find("session")->second);
    try {
        Comment::AddComment(db, std::stoi(data.find("post")->second), st.id, data.find("text")->second);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return "Comment::AddComment failed";  // Render template
    }
    return "Comment added";  // Render template
}

int PostAPI::DeletePostComment(const std::unordered_map<std::string, std::string> &data) {
    Student st = Student::GetStudentBySession(db, data.find("session")->second);
    Comment::DeleteComment(db, std::stoi(data.find("id")->second));
    return 200;
}

std::string PostAPI::Feed(const std::unordered_map<std::string, std::string> &data) {
    Student st;
    std::vector<Post> posts;
    try {
        st = Student::GetStudentBySession(db, data.find("session")->second);
    } catch (std::exception &e) {
        return templates.RenderErrors(e.what());
    }

    return templates.RenderPosts(db, st.group_id, 1, st.FullName(), st.role, Group::GetGroupName(db, st.group_id));
}
