#include <iostream>

int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;
    float angle = 0;
    float scale = 1;
    std::vector<neu::vec3> points1{ {0.5f, 0.5f, 0}, {0.5f, -0.5f, 0}, {-0.5f, -0.5f, 0}, {-0.5f, 0.5f, 0} };
    std::vector<neu::vec3> points2{ {0.75f, 0.75f, 0}, {0.75f, -0.75f, 0}, {-0.75f, -0.75f, 0}, {-0.75f, 0.75f, 0} };
    std::vector<neu::vec3> colors{ {1,1,0}, {1,0,1}, {0,1,1}, {1,1,1} };

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points1.size(), points1.data(), GL_STATIC_DRAW);


    GLuint vao;
    glGenVertexArrays(1, &vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    //vertex shader
    std::string vs_source;
    neu::file::ReadTextFile("shaders/basic.vert", vs_source);
    const char* vs_cstr = vs_source.c_str();

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vs, 1, &vs_cstr, NULL);
    glCompileShader(vs);

    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    //fragment shader
    std::string fs_source;
    neu::file::ReadTextFile("shaders/basic.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();

    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

    //glUseProgram(program);

    /*GLint uniform = glGetUniformLocation(program, "u_time");
    ASSERT(uniform != -1);*/

    float rotate = 0.0f;

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        //glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

        rotate += 1.0f * neu::GetEngine().GetTime().GetDeltaTime();

        if (rotate >= 360.0f) {
            rotate = 0.0f;
        }
        // draw
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        glPushMatrix();
        glTranslatef((neu::GetEngine().GetInput().GetMousePosition().x/(float)neu::GetEngine().GetRenderer().GetWidth()) * 2.0f - 1.0f, 1.0f - (neu::GetEngine().GetInput().GetMousePosition().y / (float)neu::GetEngine().GetRenderer().GetHeight()) * 2.0f, 0);
        glRotatef(rotate, 0.0f, 0.0f, 1.0f);
        

        glBegin(GL_QUADS);

        for (int i = 0; i < points1.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            
            glVertex3f(points1[i].x, points1[i].y, points1[i].z);
        }

        glEnd();
        glPopMatrix();
        //glBindVertexArray(vao);
        //glDrawArrays(GL_QUADS, 0, (GLsizei)points.size());
        //glBindVertexArray(0);

        /*glLoadIdentity();
        glPushMatrix();

        glTranslatef(angle, 0, 0);
        glScalef(scale, scale, 0);

        glBegin(GL_TRIANGLES);
*/
        glPushMatrix();
         
        glBegin(GL_LINE_LOOP);

        for (int i = 0; i < points2.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);

            glVertex3f(points2[i].x, points2[i].y, points2[i].z);
        }

        glEnd();

        glPopMatrix();

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
