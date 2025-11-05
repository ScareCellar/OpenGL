
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;

    auto scene = std::make_unique<neu::Scene>();
    scene->Load("scenes/scene01.json");

    auto editor = std::make_unique<neu::Editor>();
    //OPENGL Initialization
    //std::vector<neu::vec3> vertices{ {1, -1, 0}, {-1, -1, 0}, {0, 1, 0} };
    //std::vector<neu::vec3> colors{ {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
    //std::vector<neu::vec2> texcoord{ {0, 0}, {0.5f, 1}, {1, 0} };

   /*struct Vertex {
        neu::vec3 position;
        neu::vec3 color;
        neu::vec2 texcoord;
    };
    
    std::vector<Vertex> vertices{
        { { -0.5f,-0.5f,0 }, {0,1,0}, {0,0} } ,
        { {-0.5f,0.5f,0}, {0,1,0}, {0,1} },
        { {0.5f,0.5f,0}, {0,1,0}, {1,1} },
        { {0.5f,-0.5f,0}, {0,1,0}, {1,0} }
    };*/

    //std::vector<GLushort> indices{ 0,1,2 };

    //vertex buffer
    //neu::res_t<neu::VertexBuffer> vb = std::make_shared<neu::VertexBuffer>();
    /*vb->CreateVertexBuffer((GLsizei)(sizeof(Vertex)* vertices.size()), (GLsizei)vertices.size(), vertices.data());

    vb->CreateIndexBuffer(GL_UNSIGNED_SHORT, indices.size(), indices.data());
    vb->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
    vb->SetAttribute(1, 3, sizeof(Vertex), offsetof(Vertex, color));
    vb->SetAttribute(2, 2, sizeof(Vertex), offsetof(Vertex, texcoord));*/

    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/frog.obj");


    //aterial
    auto material = neu::Resources().Get<neu::Material>("materials/spot.mat");
    material->Bind();

    
    //create shaders
   /* auto vs = neu::Resources().Get<neu::Shader>("shaders/basic_lit.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic_lit.frag", GL_FRAGMENT_SHADER);*/

    //shader program
    auto program = neu::Resources().Get<neu::Program>("shaders/basic_phong.prog");
    program->Use();

    //uniform
    //GLint uniform = glGetUniformLocation(program->m_program, "u_time");

    //GLint tex_uniform = glGetUniformLocation(program->m_program, "u_texture");

    //glUniform1i(tex_uniform, 0); //GL_TEXTURE0 //replaced texture->m_texture with 0 and it worked.
    //
    //int success;

    //glGetProgramiv(program->m_program, GL_LINK_STATUS, &success);
    //if (!success)
    //{
    //    std::string infoLog(512, '\0');  // pre-allocate space
    //    GLsizei length;
    //    glGetProgramInfoLog(program->m_program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
    //    infoLog.resize(length);

    //    LOG_WARNING("Shader link failed: {}", infoLog);
    //}
    
    //texture
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/frog.jpg");
    material->program->SetUniform("u_texture", 0);

    //lights
    material->program->SetUniform("u_ambientLight", glm::vec3{ 0.5f });
    neu::Transform light{ {8,16,8} };
    glm::vec3 lightColor{ 1 };

    //transform
    neu::Transform transform{ {0,0,0} };
    neu::Transform camera{ {0,0,3} };

    //view Matrix
    float aspect = (float)neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    material->program->SetUniform("u_projection", projection);

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        scene->Update(neu::GetEngine().GetTime().GetDeltaTime());

        editor->Begin();
        editor->UpdateGui(*scene);

        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

        program->SetUniform("u_model", model);

        float dt = neu::GetEngine().GetTime().GetDeltaTime();
        
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
        
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.rotation += speed * dt;


        // fill in the rest of the controls (WS and QE)
        glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
        program->SetUniform("u_view", view);
        
        
        program->SetUniform("u_light.color", lightColor);
        //light.position.x = neu::math::sin(neu::GetEngine().GetTime().GetTime() * 10) * 5;
        program->SetUniform("u_light.position", (glm::vec3)(view* glm::vec4(light.position, 1)));



        //glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());
        

        // draw
        //neu::vec3 color{ 0,0,0 };
        neu::GetEngine().GetRenderer().Clear();
        /* vb->Draw(GL_TRIANGLES);*/
        material->UpdateGUI();
        
        material->Bind();
        model3d->Draw(GL_TRIANGLES);
        
        scene->Draw(neu::GetEngine().GetRenderer());

        // draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
