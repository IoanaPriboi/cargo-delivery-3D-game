#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

// Converteste coordonatele 2D de pe harta (mapX, mapY) in indici de matricea terrain (indexX, indexY)
bool Tema2::MapPosToIndex(int mapX, int mapY, int& indexX, int& indexY)
{
    indexX = mapX + mapCenterX;
    indexY = mapY + mapCenterY;

    if (indexX < 0 || indexX >= MAP_W) return false;
    if (indexY < 0 || indexY >= MAP_H) return false;
    return true;
}

// Converteste coordonatele 2D de pe harta (mapX, mapY) in coordonate 3D in world space
glm::vec3 Tema2::MapPosToWorldPos(int mapX, int mapY)
{
    float x = mapX * TILE_SIZE;
    float z = -mapY * TILE_SIZE;
    return glm::vec3(x, 0.0f, z);
}

// Umple un dreptunghi din matricea terrain cu tipul de teren specificat
void Tema2::FillRectMap(int xMin, int yMin, int xMax, int yMax, TerrainType type)
{
    for (int x = xMin; x <= xMax; x++) {
        for (int y = yMin; y <= yMax; y++) {
            int ix, iy;
            if (MapPosToIndex(x, y, ix, iy)) {
                terrain[ix][iy] = type;
            }
        }
    }
}

void Tema2::InitMountain()
{
    mountain.xMin = -41;
    mountain.yMin = 6;
    mountain.xMax = 4;
    mountain.yMax = 28;
    mountain.length = mountain.xMax - mountain.xMin;
    mountain.height = mountain.yMax - mountain.yMin;
}

void Tema2::InitRiver()
{
    // segment 1
    river.segments[0].xMin = -11;
    river.segments[0].yMin = -mapCenterY;
    river.segments[0].xMax = -8;
    river.segments[0].yMax = 0;
    river.segments[0].length = river.segments[0].xMax - river.segments[0].xMin;
    river.segments[0].height = river.segments[0].yMax - river.segments[0].yMin;

    // segment 2
    river.segments[1].xMin = -11;
    river.segments[1].yMin = -3;
    river.segments[1].xMax = 21;
    river.segments[1].yMax = 0;
    river.segments[1].length = river.segments[1].xMax - river.segments[1].xMin;
    river.segments[1].height = river.segments[1].yMax - river.segments[1].yMin;

    // segment 3
    river.segments[2].xMin = 18;
    river.segments[2].yMin = -3;
    river.segments[2].xMax = 21;
    river.segments[2].yMax = 14;
    river.segments[2].length = river.segments[2].xMax - river.segments[2].xMin;
    river.segments[2].height = river.segments[2].yMax - river.segments[2].yMin;

    // segment 4
    river.segments[3].xMin = 18;
    river.segments[3].yMin = 11;
    river.segments[3].xMax = mapCenterX;
    river.segments[3].yMax = 14;
    river.segments[3].length = river.segments[3].xMax - river.segments[3].xMin;
    river.segments[3].height = river.segments[3].yMax - river.segments[3].yMin;
}

void Tema2::InitTerrain()
{
    // iarba peste tot initial
    for (int x = 0; x < MAP_W; x++) {
        for (int y = 0; y < MAP_H; y++) {
            terrain[x][y] = TerrainType::GRASS;
        }
    }

    FillRectMap(mountain.xMin, mountain.yMin, mountain.xMax, mountain.yMax, TerrainType::MOUNTAIN);

    for (int i = 0; i < river.numSegments; i++) {
        auto& s = river.segments[i];
        FillRectMap(s.xMin, s.yMin, s.xMax, s.yMax, TerrainType::WATER);
    }
}


Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    //// Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

// Initializeaza mesh-ul pentru teren
void Tema2::InitTerrainMesh()
{
    // declar vectorii
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // rezerv memorie pentru viteza
    vertices.reserve(MAP_W * MAP_H * 4);
    indices.reserve(MAP_W * MAP_H * 6);

	// index pentru indici
    unsigned int idx = 0;

    // umplu vectorii cu date
    for (int iy = 0; iy < MAP_H; iy++) {
        for (int ix = 0; ix < MAP_W; ix++) {

            // calculez coordonatele
            int mapX = ix - mapCenterX;
            int mapY = iy - mapCenterY;

            float x = mapX * TILE_SIZE;
            float z = -mapY * TILE_SIZE;

            // aflu culoarea
            glm::vec3 color = GetTerrainColor(terrain[ix][iy]);

            // normala in sus
            glm::vec3 normal = glm::vec3(0, 1, 0);

            float offset = TILE_SIZE / 2.0f;

            // definesc cele 4 puncte ale patratului curent
            glm::vec3 p0 = glm::vec3(x - offset, 0, z - offset);
            glm::vec3 p1 = glm::vec3(x + offset, 0, z - offset);
            glm::vec3 p2 = glm::vec3(x + offset, 0, z + offset);
            glm::vec3 p3 = glm::vec3(x - offset, 0, z + offset);

            // adaug vertecsii in lista
            vertices.push_back(VertexFormat(p0, color, normal));
            vertices.push_back(VertexFormat(p1, color, normal));
            vertices.push_back(VertexFormat(p2, color, normal));
            vertices.push_back(VertexFormat(p3, color, normal));

            // adaug indicii pentru cele 2 triunghiuri
            indices.push_back(idx + 0);
            indices.push_back(idx + 1);
            indices.push_back(idx + 2);

            indices.push_back(idx + 0);
            indices.push_back(idx + 2);
            indices.push_back(idx + 3);

            idx += 4;
        }
    }

    // creez mesh-ul final
    CreateMesh("terrain", vertices, indices);
}

// Initializeaza mesh-ul pentru cilindru
void Tema2::InitCylinderMesh()
{
    // definesc proprietatile cilindrului
    int slices = 30;
    float height = 1.0f;
    float radius = 0.5f;
	glm::vec3 color = glm::vec3(0.5f);  // gri implicit

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // adaug centrele bazelor (jos si sus)
    // index 0: centru jos
    vertices.push_back(VertexFormat(glm::vec3(0, -height / 2, 0), color, glm::vec3(0, -1, 0)));
    
    // index 1: centru sus
    vertices.push_back(VertexFormat(glm::vec3(0, height / 2, 0), color, glm::vec3(0, 1, 0)));

	int offset = 2; // offset pentru indecsii punctelor de pe contur

    // generez punctele de pe contur
    for (int i = 0; i < slices; i++)
    {
        float angle = 2.0f * M_PI * i / slices;

		// calculez coordonatele polare x si z
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // normala pe lateral
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

        // adaug vertex pe conturul de jos
        vertices.push_back(VertexFormat(glm::vec3(x, -height / 2, z), color, normal));

        // adaug vertex pe conturul de sus
        vertices.push_back(VertexFormat(glm::vec3(x, height / 2, z), color, normal));
    }

    // construiesc fetele laterale si capacele
    for (int i = 0; i < slices; i++)
    {
        int next = (i + 1) % slices;

        // calculez indecsii punctelor de pe contur
        // offset 2 deoarece primii 2 vertecsi sunt centrele
        int bottom1 = offset + 2 * i;
        int top1 = offset + 2 * i + 1;
        int bottom2 = offset + 2 * next;
        int top2 = offset + 2 * next + 1;

        // fetele laterale (2 triunghiuri per felie)
        indices.push_back(bottom1);
        indices.push_back(bottom2);
        indices.push_back(top1);

        indices.push_back(top1);
        indices.push_back(bottom2);
        indices.push_back(top2);

        // capac jos (triunghi conectat la centru 0)
        indices.push_back(0);
        indices.push_back(bottom2);
        indices.push_back(bottom1);

        // capac sus (triunghi conectat la centru 1)
        indices.push_back(1);
        indices.push_back(top1);
        indices.push_back(top2);
    }

    // creez mesh-ul final
    CreateMesh("cylinder", vertices, indices);
}

// Initializeaza mesh-ul pentru con
void Tema2::InitConeMesh()
{
	// definesc proprietatile conului
	int slices = 30;
	float height = 1.0f;
	float radius = 0.5f;
	glm::vec3 color = glm::vec3(0.5f);  // gri implicit

	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;

	// adaug varful conului (index 0)
	vertices.push_back(VertexFormat(glm::vec3(0, height / 2, 0), color, glm::vec3(0, 1, 0)));

	// adaug centrul bazei (index 1)
	vertices.push_back(VertexFormat(glm::vec3(0, -height / 2, 0), color, glm::vec3(0, -1, 0)));

	int offset = 2; // offset pentru indicii punctelor de pe contur

	// generez punctele de pe conturul bazei
    for (int i = 0; i < slices; i++)
    {
        float angle = 2.0f * M_PI * i / slices;

        // calculez coordonatele polare x si z
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // normala pe lateral
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

        // adaug vertex pe conturul bazei
        vertices.push_back(VertexFormat(glm::vec3(x, -height / 2, z), color, normal));
	}

	// construiesc fetele laterale si capacul
    for (int i = 0; i < slices; i++)
    {
        int next = (i + 1) % slices;

		int currentIdx = offset + i;
        int nextIdx = offset + next;

        // fata laterala (triunghi conectat la varf)
        indices.push_back(0);
        indices.push_back(currentIdx);
        indices.push_back(nextIdx);

        // capac baza (triunghi conectat la centru 1)
        indices.push_back(1);
        indices.push_back(nextIdx);
		indices.push_back(currentIdx);
	}

	// creez mesh-ul final
	CreateMesh("cone", vertices, indices);
}

void Tema2::InitHexPrismMesh()
{
	// definesc proprietatile prismei hexagonale
    int slices = 6;
    float height = 1.0f;
    float radius = 0.5f;
    glm::vec3 color = glm::vec3(0.5f);  // gri implicit

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // adaug centrele bazelor (jos si sus)
    // index 0: centru jos
    vertices.push_back(VertexFormat(glm::vec3(0, -height / 2, 0), color, glm::vec3(0, -1, 0)));

    // index 1: centru sus
    vertices.push_back(VertexFormat(glm::vec3(0, height / 2, 0), color, glm::vec3(0, 1, 0)));

    int offset = 2; // offset pentru indecsii punctelor de pe contur

    // generez punctele de pe contur
    for (int i = 0; i < slices; i++)
    {
        float angle = 2.0f * M_PI * i / slices;

        // calculez coordonatele polare x si z
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // normala pe lateral
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

        // adaug vertex pe conturul de jos
        vertices.push_back(VertexFormat(glm::vec3(x, -height / 2, z), color, normal));

        // adaug vertex pe conturul de sus
        vertices.push_back(VertexFormat(glm::vec3(x, height / 2, z), color, normal));
    }

    // construiesc fetele laterale si capacele
    for (int i = 0; i < slices; i++)
    {
        int next = (i + 1) % slices;

        // calculez indecsii punctelor de pe contur
        // offset 2 deoarece primii 2 vertecsi sunt centrele
        int bottom1 = offset + 2 * i;
        int top1 = offset + 2 * i + 1;
        int bottom2 = offset + 2 * next;
        int top2 = offset + 2 * next + 1;

        // fetele laterale (2 triunghiuri per felie)
        indices.push_back(bottom1);
        indices.push_back(bottom2);
        indices.push_back(top1);

        indices.push_back(top1);
        indices.push_back(bottom2);
        indices.push_back(top2);

        // capac jos (triunghi conectat la centru 0)
        indices.push_back(0);
        indices.push_back(bottom2);
        indices.push_back(bottom1);

        // capac sus (triunghi conectat la centru 1)
        indices.push_back(1);
        indices.push_back(top1);
        indices.push_back(top2);
    }

    // creez mesh-ul final
    CreateMesh("hex_prism", vertices, indices);
}

void Tema2::InitPyramidMesh()
{
	// definesc proprietatile piramidei
    int slices = 3;
    float height = 1.0f;
    float radius = 0.5f;
    glm::vec3 color = glm::vec3(0.5f);  // gri implicit

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // adaug varful conului (index 0)
    vertices.push_back(VertexFormat(glm::vec3(0, height / 2, 0), color, glm::vec3(0, 1, 0)));

    // adaug centrul bazei (index 1)
    vertices.push_back(VertexFormat(glm::vec3(0, -height / 2, 0), color, glm::vec3(0, -1, 0)));

    int offset = 2; // offset pentru indecsii punctelor de pe contur

    // generez punctele de pe conturul bazei
    for (int i = 0; i < slices; i++)
    {
        float angle = 2.0f * M_PI * i / slices;

        // calculez coordonatele polare x si z
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // normala pe lateral
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

        // adaug vertex pe conturul bazei
        vertices.push_back(VertexFormat(glm::vec3(x, -height / 2, z), color, normal));
    }

    // construiesc fetele laterale si capacul
    for (int i = 0; i < slices; i++)
    {
        int next = (i + 1) % slices;

        int currentIdx = offset + i;
        int nextIdx = offset + next;

        // fata laterala (triunghi conectat la varf)
        indices.push_back(0);
        indices.push_back(currentIdx);
        indices.push_back(nextIdx);

        // capac baza (triunghi conectat la centru 1)
        indices.push_back(1);
        indices.push_back(nextIdx);
        indices.push_back(currentIdx);
    }

    // creez mesh-ul final
    CreateMesh("pyramid", vertices, indices);

}

void Tema2::InitHexPyramidMesh()
{
	// definesc proprietatile piramidei hexagonale
    int slices = 6;
    float height = 1.0f;
    float radius = 0.5f;
    glm::vec3 color = glm::vec3(0.5f);  // gri implicit

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // adaug varful conului (index 0)
    vertices.push_back(VertexFormat(glm::vec3(0, height / 2, 0), color, glm::vec3(0, 1, 0)));

    // adaug centrul bazei (index 1)
    vertices.push_back(VertexFormat(glm::vec3(0, -height / 2, 0), color, glm::vec3(0, -1, 0)));

    int offset = 2; // offset pentru indecsii punctelor de pe contur

    // generez punctele de pe conturul bazei
    for (int i = 0; i < slices; i++)
    {
        float angle = 2.0f * M_PI * i / slices;

        // calculez coordonatele polare x si z
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // normala pe lateral
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

        // adaug vertex pe conturul bazei
        vertices.push_back(VertexFormat(glm::vec3(x, -height / 2, z), color, normal));
    }

    // construiesc fetele laterale si capacul
    for (int i = 0; i < slices; i++)
    {
        int next = (i + 1) % slices;

        int currentIdx = offset + i;
        int nextIdx = offset + next;

        // fata laterala (triunghi conectat la varf)
        indices.push_back(0);
        indices.push_back(currentIdx);
        indices.push_back(nextIdx);

        // capac baza (triunghi conectat la centru 1)
        indices.push_back(1);
        indices.push_back(nextIdx);
        indices.push_back(currentIdx);
    }

    // creez mesh-ul final
	CreateMesh("hex_pyramid", vertices, indices);
}

void Tema2::DrawTerrain()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderMesh(meshes["terrain"], shaders["TemaShader"], modelMatrix);
}

// Returneaza tipul de mesh pentru gara data ca parametru
std::string Tema2::GetMeshNameForStation(const Station& station) {
	switch (station.resourceType)
    {
    case RESOURCE_1:
		return "box";
	case RESOURCE_2:
		return "sphere";
	case RESOURCE_3:
		return "pyramid";
	case NONE:
		return "hex_prism";
    default:
        break;
    }
}

// Returneaza inaltimea garii date ca parametru
float Tema2::GetStationBuildingHeight(const Station& station) {
    switch (station.resourceType)
    {
    case RESOURCE_1:
        return RESOURCE_STATION_HEIGHT;
    case RESOURCE_2:
        return RESOURCE_STATION_HEIGHT;
    case RESOURCE_3:
        return RESOURCE_STATION_HEIGHT;
    case NONE:
		return CENTRAL_STATION_HEIGHT;
    default:
        return 1.0f;
    }
}

// Returneaza dimeniunea garii date ca parametru
float Tema2::GetStationBuildingSize(const Station& station) {
    switch (station.resourceType)
    {
    case RESOURCE_1:
		return RESOURCE_STATION_SIZE;
    case RESOURCE_2:
        return RESOURCE_STATION_SIZE;
    case RESOURCE_3:
        return RESOURCE_STATION_SIZE;
    case NONE:
        return CENTRAL_STATION_SIZE;
    default:
        return 1.0f;
    }
}

// Returneaza tipul de teren de la pozitia data in world space
TerrainType Tema2::GetTerrainAtWorldPos(const glm::vec3& worldPos) {
    int mapX = round(worldPos.x / TILE_SIZE);
    int mapY = round(-worldPos.z / TILE_SIZE);
    
    int ix, iy;
    if (MapPosToIndex(mapX, mapY, ix, iy)) {
        return terrain[ix][iy];
    }

    return TerrainType::GRASS; // default
}

// Deseneaza cladirea garii date ca parametru
void Tema2::DrawStationBuilding(const Station& station)
{
    if (station.buildingRailId == -1) return;

    std::string meshName = GetMeshNameForStation(station);
    glm::vec3 color = GetStationRailColor(station);

    // pozitionez cladirea
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, station.buildingPosition);

    // setez dimensiunile specifice tipului de gara
    glm::vec3 scale = glm::vec3(1.0f);
    scale *= GetStationBuildingSize(station);
    scale.y = GetStationBuildingHeight(station);

    modelMatrix = glm::scale(modelMatrix, scale);

    // daca resursa e epuizata, colorez in gri
    if (station.resourceType != ResourceType::NONE && !station.hasResource) {
        color = glm::vec3(0.5f);
    }

    // verific daca e gara centrala pentru a calcula culoarea
    if (station.resourceType == ResourceType::NONE) {
        // aplic efectul de timp pentru gara centrala (se inroseste treptat in shader)
        float timeRatio = gameTimeRemaining / gameMaxTime;

        float timeColorFactor = 1.0f - timeRatio;

        if (timeColorFactor < 0.0f) timeColorFactor = 0.0f;
        if (timeColorFactor > 1.0f) timeColorFactor = 1.0f;

        GLint locSensitive = glGetUniformLocation(shaders["TemaShader"]->program, "is_time_sensitive");
        GLint locFactor = glGetUniformLocation(shaders["TemaShader"]->program, "time_mix_factor");

        glUniform1i(locSensitive, 1);
        glUniform1f(locFactor, timeColorFactor);

    }

	// desenez mesh-ul pentru corpul garii
    RenderMeshColor(meshes[meshName], shaders["TemaShader"], modelMatrix, color);

	// desenez acoperisul pentru gara centrala
    if (station.resourceType == ResourceType::NONE) {
        glm::mat4 roofMatrix = glm::mat4(1);

        // il pun la aceeasi pozitie cu cladirea
        roofMatrix = glm::translate(roofMatrix, station.buildingPosition);

		// il ridic deasupra corpului garii
        float bodyHeight = GetStationBuildingHeight(station);
        float roofHeight = 2.5f;
        roofMatrix = glm::translate(roofMatrix, glm::vec3(0, bodyHeight / 2.0f + roofHeight / 2.0f, 0));

        // il fac putin mai lat decat corpul garii
        float bodySize = GetStationBuildingSize(station);
        roofMatrix = glm::scale(roofMatrix, glm::vec3(bodySize * 1.4f, roofHeight, bodySize * 1.4f));

        // desenez acoperisul alb

		glm::vec3 roofColor = glm::vec3(1.0f, 1.0f, 1.0f);
        RenderMeshColor(meshes["hex_pyramid"], shaders["TemaShader"], roofMatrix, roofColor);
    }



    // dezactivez efectul in shader pentru urmatoarele obiecte
    if (station.resourceType == ResourceType::NONE) {
        GLint locSensitive = glGetUniformLocation(shaders["TemaShader"]->program, "is_time_sensitive");
        glUniform1i(locSensitive, 0);
    }
}

// Deseneaza toate garile
void Tema2::DrawStations()
{
	// iterez prin toate statiile
    for (int i = 0; i < 4; i++) {
        Station *station = stations[i];

		// desenez cladirea garii
		DrawStationBuilding(*station);
    }
}


// Returneaza culoarea pentru tipul de teren dat ca parametru
glm::vec3 Tema2::GetTerrainColor(TerrainType type)
{
    switch (type) {
    case TerrainType::GRASS:
        return GRASS_COLOR;
    case TerrainType::WATER:
        return WATER_COLOR;
    case TerrainType::MOUNTAIN:
        return MOUNTAIN_COLOR;
    default:
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

// Adauga o sina unidirectionala intre punctele a si b (a->b)
void Tema2::AddRail(const glm::ivec2& a, const glm::ivec2& b, RailType type)
{
    Rail rail;
    rail.id = (int)railRoad.size();
    rail.type = type;
    rail.startPosition = MapPosToWorldPos(a.x, a.y);
    rail.endPosition = MapPosToWorldPos(b.x, b.y);
    rail.centerPosition = (rail.startPosition + rail.endPosition) / 2.0f;

    rail.length = glm::length(rail.endPosition - rail.startPosition);

    rail.nextRails.clear();
    railRoad.push_back(rail);
}

// Adauga o sina bidirectionala intre punctele a si b (a<->b)
void Tema2::AddBiRail(const glm::ivec2& a, const glm::ivec2& b, RailType type)
{
    AddRail(a, b, type);
    AddRail(b, a, type);
}

// Verifica daca doua pozitii 3D sunt identice
static bool SamePos(const glm::vec3& a, const glm::vec3& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

// Construieste conexiunile dintre sine
void Tema2::BuildRailConnections()
{
	// sterg conexiunile vechi
    for (auto& r : railRoad) {
        r.nextRails.clear();
    }

	// parcurg toate perechile de sine
    for (int i = 0; i < railRoad.size(); i++)
    {
        for (int j = 0; j < railRoad.size(); j++)
        {
            if (i == j) continue;

			// verific daca sfarsitul lui i coincide cu inceputul lui j
            if (SamePos(railRoad[i].endPosition, railRoad[j].startPosition)) {
				// verific sa nu fie latura inversa
                if (SamePos(railRoad[i].startPosition, railRoad[j].endPosition))
					continue;

				// adaug conexiunea
                railRoad[i].nextRails.push_back(j);
            }
        }
    }
}

// Returneaza tipul de sina necesar pentru terenul dat ca parametru
RailType Tema2::GetRailTypeFromTerrain(TerrainType type)
{
    switch (type)
    {
    case TerrainType::GRASS:
		return RailType::NORMAL;
    case TerrainType::WATER:
		return RailType::BRIDGE;
    case TerrainType::MOUNTAIN:
		return RailType::TUNNEL;
    default:
        return RailType::NORMAL;
    }
}

// Intoarce tipul de teren de la pozitia data in coordonate 2D de harta
bool Tema2::GetTerrainAt(int mapX, int mapY, TerrainType& terrainType)
{
    int ix, iy;
    if (!MapPosToIndex(mapX, mapY, ix, iy)) return false;

    terrainType = terrain[ix][iy];

    return true;
}

// Returneaza tipul de sina necesar intre punctele a si b, in functie de teren
RailType Tema2::GetRailTypeForSegment(const glm::ivec2& a, const glm::ivec2& b)
{
    TerrainType typeA, typeB;

    // obtin tipul de teren la capete (default iarba)
    if (!GetTerrainAt(a.x, a.y, typeA)) typeA = TerrainType::GRASS;
    if (!GetTerrainAt(b.x, b.y, typeB)) typeB = TerrainType::GRASS;

    // prioritate maxima: apa -> pod
    if (typeA == TerrainType::WATER || typeB == TerrainType::WATER)
        return RailType::BRIDGE;

    // prioritate medie: munte -> tunel
    if (typeA == TerrainType::MOUNTAIN || typeB == TerrainType::MOUNTAIN)
        return RailType::TUNNEL;


    // default
    return RailType::NORMAL;
}

// Adauga o sina bidirectionala intre punctele a si b, impartind-o in segmente in functie de teren
void Tema2::AddBiRailSplitByTerrain(const glm::ivec2& a, const glm::ivec2& b)
{
    glm::ivec2 dir = glm::sign(b - a);

	// numarul de tileuri pe care le parcurg
    int steps = std::max(abs(b.x - a.x), abs(b.y - a.y));

    // retin punctul de start al segmentului curent
    glm::ivec2 segmentStart = a;
    glm::ivec2 currentPos = a;

    // determin tipul initial verificand primul pas
    RailType currentType = GetRailTypeForSegment(a, a + dir);

    for (int i = 0; i < steps; i++)
    {
        // calculez pozitia urmatoare
        glm::ivec2 nextPos = currentPos + dir;

        // determin tipul necesar pentru acest segment mic
        RailType requiredType = GetRailTypeForSegment(currentPos, nextPos);

        // daca tipul necesar difera de cel curent, intrerup segmentul
        if (requiredType != currentType)
        {
            // adaug sina acumulata pana aici
            AddBiRail(segmentStart, currentPos, currentType);

            // resetez punctul de start
            segmentStart = currentPos;

            // actualizez tipul curent
            currentType = requiredType;
        }

        // avanzez
        currentPos = nextPos;
    }

    // adaug ultima bucata ramasa pana la punctul b
    AddBiRail(segmentStart, b, currentType);
}

// Calculeaza pozitia cladirii garii si a resursei aferente
void Tema2::SetupStationBuilding(Station& station)
{
    // verific daca am o sina valida
    if (station.forwardRailId < 0 || station.forwardRailId >= railRoad.size()) return;

    const Rail& rail = railRoad[station.forwardRailId];
    station.buildingRailId = rail.id;

    // dimensiuni
    float buildingSize = GetStationBuildingSize(station);
    float buildingHeight = GetStationBuildingHeight(station);

    // calculez vectorii de directie
    glm::vec3 railDir = glm::normalize(rail.endPosition - rail.startPosition);
    glm::vec3 rightDir = glm::cross(railDir, glm::vec3(0, 1, 0)); // vectorul la dreapta (90 grade)

    // calculez offset-ul necesar pentru a nu suprapune sina
    float spacing = 2.0f;
    float offsetDist = (buildingSize + RAIL_WIDTH) / 2.0f + spacing;

    // incerc pe partea dreapta
    glm::vec3 tryPosRight = rail.centerPosition + rightDir * offsetDist;
    TerrainType tRight = GetTerrainAtWorldPos(tryPosRight);

    // incerc pe partea stanga
    glm::vec3 tryPosLeft = rail.centerPosition - rightDir * offsetDist;
    TerrainType tLeft = GetTerrainAtWorldPos(tryPosLeft);

    // decid unde pun cladirea
    // prioritizez iarba, daca dreapta e ocupata (apa/munte), merg in stanga
    if (tRight == TerrainType::GRASS) {
        station.buildingPosition = tryPosRight;
    } else if (tLeft == TerrainType::GRASS) {
        station.buildingPosition = tryPosLeft;
        // inversez sina de referinta pentru orientare corecta
        station.buildingRailId = station.backwardRailId;
    } else {
        // daca ambele sunt blocate, fortez dreapta
        station.buildingPosition = tryPosRight;
        std::cout << "Atentie: Cladirea e pe o forma de relief.\n";
    }

    // ajustez inaltimea cladirii (sa stea pe sol)
    station.buildingPosition.y += buildingHeight / 2.0f;


    // --- calcul pozitie resursa ---
    // 
    // iau sina de referinta finala (care poate fi cea backward daca am construit pe stanga)
    const Rail& refRail = railRoad[station.buildingRailId];

    // calculez vectorul right al acestei sine specifice
    // deoarece am facut inversarea (backwardRailId), right va arata mereu spre cladire
    glm::vec3 refDir = glm::normalize(refRail.endPosition - refRail.startPosition);
    glm::vec3 refRight = glm::cross(refDir, glm::vec3(0, 1, 0));

    // calculez pozitia resursei relativ la directia sinei (offset fix)
    float resourceDist = 2.0f;
    station.resourcePosition = refRail.centerPosition + (refRight * resourceDist);

    // setez inaltimea ca sa pluteasca 
    station.resourcePosition.y = RESOURCE_OFFSET_HEIGHT;
}


// Initializeaza gara centrala
void Tema2::InitCentralStation()
{
    // calculez coordonatele 3d
    centralStation.startPosition = MapPosToWorldPos(CENTRAL_STATION_POS_START.x, CENTRAL_STATION_POS_START.y);
    centralStation.endPosition = MapPosToWorldPos(CENTRAL_STATION_POS_END.x, CENTRAL_STATION_POS_END.y);
    centralStation.centerPosition = (centralStation.startPosition + centralStation.endPosition) / 2.0f;

    // adaug sina garii (bidirectionala)
    AddBiRail(CENTRAL_STATION_POS_START, CENTRAL_STATION_POS_END, RailType::STATION);

    // retin id-urile sinelor abia create (penultima si ultima din vector)
    centralStation.forwardRailId = (int)railRoad.size() - 2;
    centralStation.backwardRailId = (int)railRoad.size() - 1;

    centralStation.resourceType = ResourceType::NONE;

    // calculez pozitia cladirii fata de sina
    SetupStationBuilding(centralStation);

    // generez prima comanda de resurse
    GenerateOrder();
}

// Initializeaza statiile de resurse
void Tema2::InitResourceStations()
{
    // --- statia 1 ---
    resourceStation1.startPosition = MapPosToWorldPos(RESOURCE_STATION_1_POS_START.x, RESOURCE_STATION_1_POS_START.y);
    resourceStation1.endPosition = MapPosToWorldPos(RESOURCE_STATION_1_POS_END.x, RESOURCE_STATION_1_POS_END.y);
    resourceStation1.centerPosition = (resourceStation1.startPosition + resourceStation1.endPosition) * 0.5f;

    AddBiRail(RESOURCE_STATION_1_POS_START, RESOURCE_STATION_1_POS_END, RailType::STATION);

    resourceStation1.forwardRailId = (int)railRoad.size() - 2;
    resourceStation1.backwardRailId = (int)railRoad.size() - 1;
    resourceStation1.resourceType = ResourceType::RESOURCE_1;

    SetupStationBuilding(resourceStation1);


    // --- statia 2 ---
    resourceStation2.startPosition = MapPosToWorldPos(RESOURCE_STATION_2_POS_START.x, RESOURCE_STATION_2_POS_START.y);
    resourceStation2.endPosition = MapPosToWorldPos(RESOURCE_STATION_2_POS_END.x, RESOURCE_STATION_2_POS_END.y);
    resourceStation2.centerPosition = (resourceStation2.startPosition + resourceStation2.endPosition) * 0.5f;

    AddBiRail(RESOURCE_STATION_2_POS_START, RESOURCE_STATION_2_POS_END, RailType::STATION);

    resourceStation2.forwardRailId = (int)railRoad.size() - 2;
    resourceStation2.backwardRailId = (int)railRoad.size() - 1;
    resourceStation2.resourceType = ResourceType::RESOURCE_2;

    SetupStationBuilding(resourceStation2);


    // --- statia 3 ---
    resourceStation3.startPosition = MapPosToWorldPos(RESOURCE_STATION_3_POS_START.x, RESOURCE_STATION_3_POS_START.y);
    resourceStation3.endPosition = MapPosToWorldPos(RESOURCE_STATION_3_POS_END.x, RESOURCE_STATION_3_POS_END.y);
    resourceStation3.centerPosition = (resourceStation3.startPosition + resourceStation3.endPosition) * 0.5f;

    AddBiRail(RESOURCE_STATION_3_POS_START, RESOURCE_STATION_3_POS_END, RailType::STATION);

    resourceStation3.forwardRailId = (int)railRoad.size() - 2;
    resourceStation3.backwardRailId = (int)railRoad.size() - 1;
    resourceStation3.resourceType = ResourceType::RESOURCE_3;

    SetupStationBuilding(resourceStation3);
}

/* Initializeaza sinele */
void Tema2::InitRails()
{
	// sterg sinele vechi
    railRoad.clear();

    AddBiRailSplitByTerrain({ -0, -7 }, { 11, -7 });
    AddBiRailSplitByTerrain({ 11, -7 }, { 11, 3 });
	AddBiRailSplitByTerrain({ 11, 3 }, { 11, 17 });
	AddBiRailSplitByTerrain({ 11, 17 }, { 21, 17 });
    AddBiRailSplitByTerrain({ 25, 17 }, { 34, 17 });
	AddBiRailSplitByTerrain({ 34, 17 }, { 34, 3 });
	AddBiRailSplitByTerrain({ 34, 3 }, { 11, 3 });
	AddBiRailSplitByTerrain({ 11, 3 }, { -16, 3 });
	AddBiRailSplitByTerrain({ -16, 3 }, { -16, -7 });
	AddBiRailSplitByTerrain({ -16, -7 }, { -4, -7});
	AddBiRailSplitByTerrain({ -16, -7 }, { -25, -7});
	AddBiRailSplitByTerrain({ -25, -7 }, { -25, 17 });
	AddBiRailSplitByTerrain({ -25, 17 }, { 11, 17 });
	AddBiRailSplitByTerrain({ -25, -7 }, { -34, -7 });
	AddBiRailSplitByTerrain({ -34, -7 }, { -34, -16 });
	AddBiRailSplitByTerrain({ -34, -16 }, { -25, -16 });
	AddBiRailSplitByTerrain({ -25, -16 }, { -25, -7 });
    AddBiRailSplitByTerrain({ -25, -16 }, { -15, -16 });
	AddBiRailSplitByTerrain({ -15, -16 }, { -15, -21 });
	AddBiRailSplitByTerrain({ -15, -21 }, { -15, -26 });
	AddBiRailSplitByTerrain({ -15, -26 }, { -18, -26 });
	AddBiRailSplitByTerrain({ -22, -26 }, { -25, -26 });
	AddBiRailSplitByTerrain({ -25, -26 }, { -25, -16 });
    AddBiRailSplitByTerrain({ -15, -21 }, { 11, -21 });
	AddBiRailSplitByTerrain({ 11, -21 }, { 11, -7 });
    AddBiRailSplitByTerrain({ 11, -21 }, { 34, -21 });
	AddBiRailSplitByTerrain({ 34, -21 }, { 44, -21 });
	AddBiRailSplitByTerrain({ 44, -21 }, { 44, -23 });
	AddBiRailSplitByTerrain({ 44, -27 }, { 44, -29 });
	AddBiRailSplitByTerrain({ 44, -29 }, { 34, -29 });
	AddBiRailSplitByTerrain({ 34, -29 }, { 34, -21 });
	AddBiRailSplitByTerrain({ 34, -21 }, { 34, -7 });
    AddBiRailSplitByTerrain({ 34, -7 }, { 34, 3 });
	AddBiRailSplitByTerrain({ 34, -7 }, { 11, -7 });

	// initializez garile cu sinele lor
	InitCentralStation();
	InitResourceStations();

	// construiesc conexiunile dintre sine
    BuildRailConnections();
}


// Returneaza culoarea pentru tipul de sina dat ca parametru
glm::vec3 Tema2::GetRailColor(RailType type)
{
    switch (type) {
    case RailType::NORMAL:
        return NORMAL_RAIL_COLOR;
    case RailType::BRIDGE:
        return BRIDGE_RAIL_COLOR;
    case RailType::TUNNEL:
        return TUNNEL_RAIL_COLOR;
	case RailType::STATION:
		return STATION_RAIL_COLOR;
    default:
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

// Calculeaza matricea de transformare (pozitie + rotatie) pentru o sina
glm::mat4 Tema2::ComputeRailModelMatrix(const Rail& rail)
{
    // calculez centrul geometric
    glm::vec3 center = (rail.startPosition + rail.endPosition) * 0.5f;

    // ridic sina la inaltimea corecta
    center.y += (RAIL_HEIGHT / 2.0f);

    // calculez directia si unghiul de rotatie
    glm::vec3 dir = rail.endPosition - rail.startPosition;
    float angle = atan2(dir.z, dir.x);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, center);

    // rotesc sina pentru a se alinia cu directia vectorului (invers trigonometric)
    modelMatrix = glm::rotate(modelMatrix, -angle, glm::vec3(0, 1, 0));

    return modelMatrix;
}

// Deseneaza o sina normala
void Tema2::DrawNormalRail(const Rail& rail)
{
    glm::mat4 modelMatrix = ComputeRailModelMatrix(rail);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(rail.length, RAIL_HEIGHT, RAIL_WIDTH));
    RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, NORMAL_RAIL_COLOR);
}

// Returneaza culoarea pentru sina unei gari, in functie de tipul de resursa
glm::vec3 Tema2::GetStationRailColor(const Station& station)
{
    switch (station.resourceType) 
    {
    case ResourceType::NONE:
        return CENTRAL_STATION_COLOR;
    case ResourceType::RESOURCE_1:
        return RESOURCE_1_COLOR;
    case ResourceType::RESOURCE_2:
        return RESOURCE_2_COLOR;
    case ResourceType::RESOURCE_3:
        return RESOURCE_3_COLOR;
    default:
        return STATION_RAIL_COLOR;
    }
}

// Returneaza statia asociata unei sine date prin id
Station* Tema2::GetStationByRailId(int railId)
{
    for (int i = 0; i < 4; i++) {
        Station* s = stations[i];

        if (s->forwardRailId == railId || s->backwardRailId == railId) {
            return s;
        }
    }
    return nullptr;
}

// Deseneaza o sina din gara
void Tema2::DrawStationRail(const Rail& rail)
{
    Station* station = GetStationByRailId(rail.id);

    glm::mat4 modelMatrix = ComputeRailModelMatrix(rail);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(rail.length, RAIL_HEIGHT, RAIL_WIDTH));

    RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, GetStationRailColor(*station));
}

// Deseneaza o sina tip pod
void Tema2::DrawBridgeRail(const Rail& rail)
{
	glm::mat4 baseModelMatrix = ComputeRailModelMatrix(rail);

	// culorile alternante pentru segmente
	glm::vec3 color1 = BRIDGE_RAIL_COLOR;
	glm::vec3 color2 = BRIDGE_RAIL_COLOR * 0.8f;

    // desenez 4 segmente longitudinale, alipite, cu putin spatiu intre ele
	int numSegments = 4;
    float spacing = RAIL_WIDTH * 0.04f;
	float totalSpacing = spacing * (numSegments - 1);

	float segmentWidth = (RAIL_WIDTH - totalSpacing) / numSegments;

	// centrul primei bucati pe axa Z
	float startZ = -RAIL_WIDTH * 0.5f + segmentWidth * 0.5f;

    for (int i = 0; i < numSegments; i++) {
        glm::mat4 modelMatrix = baseModelMatrix;

		float z = startZ + i * (segmentWidth + spacing);

		// pozitionez si scalez segmentul
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(rail.length, RAIL_HEIGHT, segmentWidth));

		// calculez culoarea in functie de index
        glm::vec3 color = (i % 2 == 0) ? color1 : color2;

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, color);
	}

}

// Deseneaza o sina tip tunel
void Tema2::DrawTunnelRail(const Rail& rail)
{
	glm::mat4 baseModelMatrix = ComputeRailModelMatrix(rail);

    glm::vec3 color1 = TUNNEL_RAIL_COLOR;
    glm::vec3 color2 = TUNNEL_RAIL_COLOR * 0.8f;

    // lungimea fixa a unui segment de tunel
    float segmentLen = 0.5f * TILE_SIZE;

    // spatiu mic intre segmente
    float spacing = 0.07f * segmentLen;

    // numarul de segmente necesare
    int numSegments = (int) (rail.length / (segmentLen + spacing));
    if (numSegments < 1) numSegments = 1;


	// pozitia centrului primului segment
	float startX = -rail.length * 0.5f + segmentLen * 0.5f;

    for (int i = 0; i < numSegments; i++) {
        glm::mat4 modelMatrix = baseModelMatrix;

        float x = startX + i * (segmentLen + spacing);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(segmentLen, RAIL_HEIGHT, RAIL_WIDTH));

        glm::vec3 color = (i % 2 == 0) ? color1 : color2;
        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, color);
	}
}


// Deseneaza toate sinele
void Tema2::DrawRails()
{
	// desenez doar sinele pare (pt ca sunt bidirectionale)
	for (int i = 0; i < railRoad.size(); i += 2) {
		const Rail& r = railRoad[i];
        switch (r.type) {
        case RailType::NORMAL:
            DrawNormalRail(r);
            break;
        case RailType::BRIDGE:
            DrawBridgeRail(r);
            break;
        case RailType::TUNNEL:
            DrawTunnelRail(r);
            break;
        case RailType::STATION:
            DrawStationRail(r);
            break;
        default:
            break;
        }
	}
}


// Genereaza comanda de 5 resurse la Gara Centrala
void Tema2::GenerateOrder()
{
    // setez timpul in functie de dificultate
    switch (gameDifficulty)
    {
    case m1::EASY:
        gameMaxTime = GAME_TIME_EASY;
        break;
    case m1::MEDIUM:
		gameMaxTime = GAME_TIME_MEDIUM;
        break;
    case m1::HARD:
		gameMaxTime = GAME_TIME_HARD;
        break;
    default:
        break;
    }

	// generez random comanda, tinand cont de nivelul de dificultate
	centralStationOrder.clear();
    while (centralStationOrder.size() < 5) {
        ResourceType rt = (ResourceType)((rand() % 3) + 1);

        // maxim 2 resurse de acelasi fel pentru MEDIUM
        if (gameDifficulty == GameDifficulty::MEDIUM) {
            int count = 0;

            // numar cate am deja de tipul asta
            for (ResourceType existing : centralStationOrder) {
                if (existing == rt) count++;
            }

            // daca am deja 2, sar peste si incerc din nou
            if (count >= 2) continue;
        }

        centralStationOrder.push_back(rt);
    }

	// afisez comanda in consola
    std::cout << "==============================================\n";
    std::cout << "Comanda generata la Gara Centrala:\n";
    for (ResourceType rt : centralStationOrder) {
        switch (rt) {
        case ResourceType::RESOURCE_1:
			cout << "Ornament 1 (mov) \n";
            break;
        case ResourceType::RESOURCE_2:
            std::cout << "Ornament 2 (albastru) \n";
            break;
        case ResourceType::RESOURCE_3:
            std::cout << "Ornament 3 (galben) \n";
            break;
        default:
            break;
        }
	}
	cout << "==============================================\n";

	// resetez timpul ramas
	gameTimeRemaining = gameMaxTime;
}

// Functie de debug pentru afisarea informatiilor despre sine
void Tema2::PrintRailsInfo()
{
    std::cout << "Numar sine: " << railRoad.size() << "\n";
    for (const Rail& r : railRoad) {
        std::cout << "Rail " << r.id << ": type=" << (int)r.type
            << " start=(" << r.startPosition.x + 50 << "," << r.startPosition.y << "," << r.startPosition.z + 50 << ")"
            << " end=(" << r.endPosition.x + 50 << "," << r.endPosition.y << "," << r.endPosition.z + 50 <<")"
            << " len=" << r.length
            << " nexts=[";
        for (int nx : r.nextRails) {
            std::cout << nx << " ";
        }
        std::cout << "]\n";
    }
}

// Returneaza id-ul sinei urmatoare in functie de tasta apasata
int Tema2::FindNextRail(char command)
{
    // directia curenta a trenului (normalizata)
    glm::vec3 trainDir = glm::normalize(train.direction);

    for (int railId : train.possibleNextRails)
    {
        const Rail& r = railRoad[railId];

        // calculez directia noii sine candidate
        glm::vec3 railDir = glm::normalize(r.endPosition - r.startPosition);

        // folosesc produsul scalar pentru a afla cat de paralele sunt (cosinus)
        float cosAngle = glm::dot(trainDir, railDir);

        // folosesc produsul vectorial pe axa Y pentru a determina semnul (sinus)
        glm::vec3 crossProduct = glm::cross(trainDir, railDir);
        float sinAngle = crossProduct.y;

		// calculez unghiul intre cele doua directii
        float angleRad = atan2(sinAngle, cosAngle);
        float angleDeg = glm::degrees(angleRad);

		// verific tasta apasata si unghiul
        switch (command)
        {
        case 'W': // inainte
            if (angleDeg > -45.0f && angleDeg < 45.0f) {
                return railId;
            }
            break;
        case 'S': // inapoi
            if (angleDeg < -135.0f || angleDeg > 135.0f) {
                return railId;
            }
            break;
        case 'A': // stanga
            if (angleDeg > 45.0f && angleDeg < 135.0f) {
                return railId;
            }
            break;
        case 'D': // dreapta
            if (angleDeg < -45.0f && angleDeg > -135.0f) {
                return railId;
            }
            break;
        default:
            break;
		}
    }

    // daca am parcurs toate sinele si niciuna nu se potriveste cu tasta apasata
    return -1;
}

// Gestioneaza toata logica de miscare si interactiune a trenului
void Tema2::MoveTrain(float deltaTimeSeconds)
{
	// Logica de livrare a resurselor la gara centrala
    // verific timer-ul pentru livrare
    if (train.waitTimer > 0.0f)
    {
        train.waitTimer -= deltaTimeSeconds;
        if (train.waitTimer < 0.0f) {

            train.waitTimer = 0.0f;

            train.state = TrainState::WAITING_IN_STATION;

            const Rail& r = railRoad[train.currentRail];
            train.possibleNextRails = r.nextRails;

            // mut resursele colectate in brad sub forma de globuri
            for (ResourceType res : train.collectedResources) {
                Ornament o;
                o.type = res;

                // aleg un nivel random (0, 1 sau 2)
                int level = rand() % 3;

                // generez un offset random pentru inaltime (intre 0.0 si 0.8)
				// => globul poate fi la baza etajului sau putin mai sus
                float heightOffset = ((rand() % 100) / 100.0f) * 0.8f;

                float radius = 0.0f;
                float height = 0.0f;

				// unde incepe primul etaj
                float startY = TREE_TRUNK_HEIGHT - 0.5f;

				// diferenta de inaltime intre etaje
                float step = TREE_LEVEL_HEIGHT * 0.7f;

                if (level == 0) { // jos
                    // raza de baza a nivelului
                    float baseRadius = TREE_L1_SCALE * 0.5f;

					// calculez inaltimea la care pun ornamentul
                    height = startY + 0.3f + heightOffset; // +0.3 e baza minima

					// ajustez raza in functie de inaltimea la care pun ornamentul
                    radius = (baseRadius - heightOffset * 0.5f) * 1;
                }
                else if (level == 1) { // mijloc
					startY += step;
                    float baseRadius = TREE_L2_SCALE * 0.5f;

                    height = startY + 0.3f + heightOffset;
                    radius = (baseRadius - heightOffset * 0.5f) * 1;
                }
                else { // sus
					startY += 2 * step;
                    float baseRadius = TREE_L3_SCALE * 0.5f;

                    height = startY + 0.3f + heightOffset;

					// 0.7 pt ca e mai ascutit varful
                    radius = (baseRadius - heightOffset * 0.7f) * 1;
                }

                // aleg un unghi random (0 - 360 grade)
                float angle = ((rand() % 360)) * (M_PI / 180.0f);

                // clculez pozitia finala
                o.relativePos = glm::vec3(radius * cos(angle), height, radius * sin(angle));

                treeOrnaments.push_back(o);
            }

			totalScore += gameTimeRemaining * SCORE_PER_SECOND;
			collectedOrnamentsCount += (int)train.collectedResources.size();

            train.collectedResources.clear();

            // daca am livrat la gara centrala, generez o noua comanda
            if (centralStationOrder.empty()) {
                GenerateOrder();
                std::cout << "Livrare finalizata! O noua comanda a fost generata.\n";
            }
        }
        return;
	}


    // Logica pentru procesul de incarcare (animatia carligului)
    if (train.state == TrainState::LOADING_RESOURCE)
    {
        // initializez starea carligului daca abia am intrat in acest mod
        if (train.hookState == HookState::IDLE) {
            train.hookState = HookState::EXTENDING;
			train.hookTipPosition = train.position + HOOK_OFFSET;

            const Rail& currentRailObj = railRoad[train.currentRail];
            Station* s = GetStationByRailId(currentRailObj.id);

            train.hookTargetPosition = s->resourcePosition;
        }

		glm::vec3 hookOrigin = train.position + HOOK_OFFSET;
        glm::vec3 direction = glm::normalize(train.hookTargetPosition - hookOrigin);

		// extinderea carligului
        if (train.hookState == HookState::EXTENDING)
        {
            // deplasez varful spre tinta
            train.hookTipPosition += direction * train.hookSpeed * deltaTimeSeconds;

            BoundingBox hookBox(train.hookTipPosition, glm::vec3(HOOK_SIZE));
            BoundingBox resourceBox(train.hookTargetPosition, glm::vec3(RESOURCE_SIZE));

            bool collision = CheckCollision(hookBox, resourceBox);

			// daca am atins resursa sau tinta retrag carligul
            if (collision)
            {
                train.hookState = HookState::RETRACTING;

                const Rail& currentRailObj = railRoad[train.currentRail];
                Station* s = GetStationByRailId(currentRailObj.id);

                if (s) {
                    // colectez resursa
                    if (s->resourceType != ResourceType::NONE && s->hasResource) {
                        CollectResource(s->resourceType);
                        s->hasResource = false;
                        s->regenerationTimer = RESOURCE_REGEN_TIME;
                    }
                }
            }
        }
		// retragerea carligului
        else if (train.hookState == HookState::RETRACTING)
        {
            // retrag carligul spre tren
            train.hookTipPosition -= direction * train.hookSpeed * deltaTimeSeconds;
            float distToOrigin = glm::length(train.hookTipPosition - hookOrigin);

            if (distToOrigin < 0.1f) {
                train.hookState = HookState::IDLE;
                const Rail& currentRailObj = railRoad[train.currentRail];
                train.state = TrainState::WAITING_IN_STATION;
                train.possibleNextRails = currentRailObj.nextRails;

                return;
            }
        }
        return;
    }

    // Logica de asteptare in gara (verific daca apare resursa cat timp stau)
    if (train.state == TrainState::WAITING_IN_STATION)
    {
        const Rail& currentRailObj = railRoad[train.currentRail];
        Station* s = GetStationByRailId(currentRailObj.id);

        if (s && s->resourceType != ResourceType::NONE &&
            s->hasResource && IsResourceNeeded(s->resourceType))
        {
            train.state = TrainState::LOADING_RESOURCE;
            std::cout << "Resursa regenerata! Incep incarcarea...\n";
        }
        return;
    }

    // Daca astept input la intersectie, nu fac nimic
    if (train.state == TrainState::WAITING_AT_INTERSECTION ||
        train.state == TrainState::WAITING_START)
    {
        return;
    }

    // Logica de miscare efectiva pe sina
    const Rail& currentRailObj = railRoad[train.currentRail];
    float deltaProgress = (train.speed * deltaTimeSeconds) / currentRailObj.length;
    train.progress += deltaProgress;

    // Verific daca am ajuns la capatul sinei curente
    if (train.progress >= 1.0f)
    {
        train.progress = 1.0f;
        Station* station = GetStationByRailId(currentRailObj.id);

        // cazul 1: am ajuns la o statie de resurse si am nevoie de ea
        if (station && station->resourceType != ResourceType::NONE &&
            station->hasResource && IsResourceNeeded(station->resourceType))
        {
            train.state = TrainState::LOADING_RESOURCE;
            std::cout << "Ajuns in gara. Incarc resursa...\n";
            return;
        }
        // cazul 2: am ajuns la gara centrala pentru livrare
        else if (station && station->resourceType == ResourceType::NONE &&
            centralStationOrder.empty())
        {
            train.state = TrainState::LOADING_RESOURCE;
            train.waitTimer = RESOURCE_LOAD_TIME;
            std::cout << "Ajuns la Gara Centrala. Descarc marfa...\n";
            return;
        }

        // cazul 3: intersectie (mai multe cai posibile)
        if (currentRailObj.nextRails.size() != 1)
        {
            if (currentRailObj.type == RailType::STATION)
                train.state = TrainState::WAITING_IN_STATION;
            else
                train.state = TrainState::WAITING_AT_INTERSECTION;

            train.possibleNextRails = currentRailObj.nextRails;

            if (currentRailObj.nextRails.empty()) train.speed = 0.0f;
            return;
        }

        // cazul 4: o singura continuare (merg automat mai departe)
        {
            int nextRailId = currentRailObj.nextRails[0];
            train.currentRail = nextRailId;
            train.possibleNextRails = railRoad[nextRailId].nextRails;
            train.progress = 0.0f;
        }
    }

	// actualizez pozitia si directia trenului
    const Rail& r = railRoad[train.currentRail];
    train.position = glm::mix(r.startPosition, r.endPosition, train.progress);  // lerp
    train.direction = glm::normalize(r.endPosition - r.startPosition);
}

// Deseneaza carligul trenului si resursa prinsa (daca e cazul)
void Tema2::DrawHook()
{
    if (train.state != TrainState::LOADING_RESOURCE || train.hookState == IDLE) return;

    const Rail& currentRailObj = railRoad[train.currentRail];
    Station* s = GetStationByRailId(currentRailObj.id);

    // daca sunt la gara centrala nu desenez
    if (!s || s->resourceType == ResourceType::NONE) return;

    // calculez pozitiile de start si final
	glm::vec3 startPos = train.position + HOOK_OFFSET;

    glm::vec3 endPos = train.hookTipPosition;

    // calculez vectorul si lungimea bratului
    glm::vec3 hookVector = endPos - startPos;
    float length = glm::length(hookVector);

    // calculez unghiul orizontal
    glm::vec3 dir = glm::normalize(hookVector);
    float angle = atan2(dir.z, dir.x);

    // centrul bratului
    glm::vec3 center = (startPos + endPos) * 0.5f;

    // desenez bratul carligului
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, center);
        modelMatrix = glm::rotate(modelMatrix, -angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(length, HOOK_SIZE, HOOK_SIZE));

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
    }

    // desenez resursa doar daca am prins-o (cand carligul se retrage)
    if (train.hookState == HookState::RETRACTING)
    {
        // identific gara curenta pentru a sti ce resursa desenez
        const Rail& currentRailObj = railRoad[train.currentRail];
        Station* s = GetStationByRailId(currentRailObj.id);

        if (s) {
            // aflu numele mesh-ului si culoarea
            std::string meshName = GetMeshNameForStation(*s);
            glm::vec3 color = GetStationRailColor(*s);

            // desenez varful carligului (resursa)
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, endPos);
            modelMatrix = glm::rotate(modelMatrix, -angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(RESOURCE_SIZE));

            RenderMeshColor(meshes[meshName], shaders["TemaShader"], modelMatrix, color);
        }
    }
}

// Initializeaza trenul
void Tema2::InitTrain()
{
	// trenul porneste de la gara centrala
    int startRailId = centralStation.forwardRailId;


    // setez proprietatile de baza ale trenului
    train.currentRail = startRailId;
    train.progress = 1.0f;           // incepe la capatul sinei
    train.speed = 9.0f;              // viteza de deplasare
    train.state = TrainState::WAITING_START; // porneste in asteptare

    // calculez pozitia 3D exacta
    const Rail& r = railRoad[train.currentRail];
    train.position = glm::mix(r.startPosition, r.endPosition, train.progress);

    // calculez directia de orientare
    train.direction = glm::normalize(r.endPosition - r.startPosition);

    // setez optiunile pentru urmatoarea intersectie
    train.possibleNextRails = r.nextRails;

	// setez starea carligului
    train.hookState = HookState::IDLE;
    
}

// Deseneaza trenul
void Tema2::DrawTrain()
{
    // calculez unghiul de rotatie in functie de directie
    float angle = atan2(train.direction.z, train.direction.x);

    // calculez inaltimea la care stau rotile (pe sina)
    float railTopY = RAIL_HEIGHT;
    float wheelCenterY = railTopY + WHEEL_RADIUS;

    // matricea de baza a trenului (pozitie + rotatie)
    glm::mat4 baseMatrix = glm::mat4(1);
    baseMatrix = glm::translate(baseMatrix, train.position);
    baseMatrix = glm::rotate(baseMatrix, -angle, glm::vec3(0, 1, 0));

    // --- LOCOMOTIVA ---

    // desenez rotile locomotivei (4 perechi)
    int numWheelsLoco = 4;
    float distBetweenWheels = (LOCOMOTIVE_LENGTH * 0.8f) / (numWheelsLoco - 1);
    float startX = -(LOCOMOTIVE_LENGTH * 0.8f) / 2.0f;

    for (int i = 0; i < numWheelsLoco; i++)
    {
        float xOffset = startX + i * distBetweenWheels;

        // roata stanga
        glm::mat4 modelMatrix = baseMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xOffset, wheelCenterY, (LOCOMOTIVE_WIDTH / 2.0f) - (WHEEL_WIDTH / 2.0f)));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2 * WHEEL_RADIUS, WHEEL_WIDTH, 2 * WHEEL_RADIUS));
        RenderMeshColor(meshes["cylinder"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_WHEEL);

        // roata dreapta
        modelMatrix = baseMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xOffset, wheelCenterY, -(LOCOMOTIVE_WIDTH / 2.0f) + (WHEEL_WIDTH / 2.0f)));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2 * WHEEL_RADIUS, WHEEL_WIDTH, 2 * WHEEL_RADIUS));
        RenderMeshColor(meshes["cylinder"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_WHEEL);
    }

    // desenez sasiul locomotivei
    float chassisY = wheelCenterY + WHEEL_RADIUS + CHASSIS_HEIGHT / 2.0f;
    {
        glm::mat4 modelMatrix = baseMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, chassisY, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(LOCOMOTIVE_LENGTH, CHASSIS_HEIGHT, LOCOMOTIVE_WIDTH));
        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_CHASSIS);
    }

    // desenez corpul locomotivei
    float bodyBottomY = chassisY + CHASSIS_HEIGHT / 2.0f;
    float cabinLen = LOCOMOTIVE_LENGTH * LOCO_CABIN_RATIO;
    float engineLen = LOCOMOTIVE_LENGTH * (1.0f - LOCO_CABIN_RATIO);

    // cabina vatmanului (spate)
    float cabinX = -LOCOMOTIVE_LENGTH / 2.0f + cabinLen / 2.0f;
    {
        glm::mat4 modelMatrix = baseMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(cabinX, bodyBottomY + CABIN_HEIGHT / 2.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(cabinLen, CABIN_HEIGHT, LOCOMOTIVE_WIDTH));
        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_BODY);
    }

    // motorul (fata)
    float engineX = (LOCOMOTIVE_LENGTH / 2.0f) - (engineLen / 2.0f);
    float engineCenterY = bodyBottomY + ENGINE_DIAMETER / 2.0f;
    {
        glm::mat4 modelMatrix = baseMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(engineX, engineCenterY, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(ENGINE_DIAMETER, engineLen, ENGINE_DIAMETER));
        RenderMeshColor(meshes["cylinder"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_ENGINE);
    }

    // detaliu motor (mic cilindru in fata)
    {
        glm::mat4 modelMatrix = baseMatrix;
        float detailX = (LOCOMOTIVE_LENGTH / 2.0f) + (DETAIL_LENGTH / 2.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(detailX, engineCenterY, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2 * DETAIL_RADIUS, DETAIL_LENGTH, 2 * DETAIL_RADIUS));
        RenderMeshColor(meshes["cylinder"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_DETAIL);
    }

    // --- VAGON ---

    // calculez pozitia vagonului (in spate)
    float distLocoToWagon = (LOCOMOTIVE_LENGTH / 2.0f) + COUPLING_DISTANCE + (WAGON_LENGTH / 2.0f);
    glm::mat4 wagonMatrix = glm::translate(baseMatrix, glm::vec3(-distLocoToWagon, 0, 0));

    // desenez rotile vagonului (2 perechi)
    float wagonWheelOffset = WAGON_LENGTH * 0.3f;
    for (int i = -1; i <= 1; i += 2)
    {
        float xPos = i * wagonWheelOffset;

        // roata stanga
        glm::mat4 modelMatrix = wagonMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, wheelCenterY, (WAGON_WIDTH / 2.0f) - (WHEEL_WIDTH / 2.0f)));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2 * WHEEL_RADIUS, WHEEL_WIDTH, 2 * WHEEL_RADIUS));
        RenderMeshColor(meshes["cylinder"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_WHEEL);

        // roata dreapta
        modelMatrix = wagonMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, wheelCenterY, -(WAGON_WIDTH / 2.0f) + (WHEEL_WIDTH / 2.0f)));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2 * WHEEL_RADIUS, WHEEL_WIDTH, 2 * WHEEL_RADIUS));
        RenderMeshColor(meshes["cylinder"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_WHEEL);
    }

    // desenez sasiul vagonului
    {
        glm::mat4 modelMatrix = wagonMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, chassisY, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(WAGON_LENGTH, CHASSIS_HEIGHT, WAGON_WIDTH));
        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_CHASSIS);
    }

    // desenez corpul vagonului
    {
        glm::mat4 modelMatrix = wagonMatrix;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, bodyBottomY + WAGON_HEIGHT / 2.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(WAGON_LENGTH, WAGON_HEIGHT, WAGON_WIDTH));
        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, TRAIN_COLOR_BODY);
    }

	// --- RESURSE COLECTATE ---
	// definesc dimensiunea si spatiul dintre resurse
    float miniResourceSize = RESOURCE_SIZE * 0.6f;
    float spacing = 0.1f + miniResourceSize;

    // calculez pozitia pentru resursa, centrata in vagon
    float resourcesStartY = bodyBottomY + WAGON_HEIGHT / 2;
    float totalLen = (train.collectedResources.size() - 1) * spacing;
    float startResX = -totalLen / 2.0f;

	// iterez prin toate resursele colectate si le desenez
    for (int i = 0; i < train.collectedResources.size(); i++) {

        ResourceType res = train.collectedResources[i];

		// calculez offset-ul pe axa X
        float xOffset = startResX + i * spacing;

		// pornesc de la matricea vagonului
        glm::mat4 resMatrix = wagonMatrix;

        // pun resursa in vagon
        resMatrix = glm::translate(resMatrix, glm::vec3(xOffset, resourcesStartY, 0));
		resMatrix = glm::scale(resMatrix, glm::vec3(miniResourceSize));

		// determin mesh-ul si culoarea
        std::string mesh = GetResourceMesh(res);
        glm::vec3 color = GetResourceColor(res);


        RenderMeshColor(meshes[mesh], shaders["TemaShader"], resMatrix, color);
    }
}

// Verifica coliziunea intre doua AABB
bool Tema2::CheckCollision(const BoundingBox& a, const BoundingBox& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

// Returneaza culoarea pentru resursa unei gari
glm::vec3 Tema2::GetResourceColor(ResourceType rt)
{
    // culoare mai deschisa decat cea a garii
	float colorFactor = 1.4f;

    switch (rt)
    {
    case m1::RESOURCE_1:
		return RESOURCE_1_COLOR * colorFactor;
    case m1::RESOURCE_2:
        return RESOURCE_2_COLOR * colorFactor;
    case m1::RESOURCE_3:
		return RESOURCE_3_COLOR * colorFactor;
    default:
		return glm::vec3(1);
    }
}

// Returneaza mesh-ul pentru resursa unei gari
std::string Tema2::GetResourceMesh(ResourceType rt)
{
    switch (rt)
    {
    case m1::RESOURCE_1:
		return "box";
    case m1::RESOURCE_2:
		return "sphere";
    case m1::RESOURCE_3:
		return "pyramid";
    default:
        return "box";
    }
}

// Deseneaza resursa unei gari
void Tema2::DrawResource(const Station& s, float rotationAngle)
{
	if (s.resourceType == ResourceType::NONE || !s.hasResource) return;

    // folosesc pozitia calculata la initializare
    glm::vec3 resourcePos = s.resourcePosition;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, resourcePos);
    modelMatrix = glm::rotate(modelMatrix, rotationAngle, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(RESOURCE_SIZE));

    // determin mesh-ul si culoarea
    std::string meshName = GetMeshNameForStation(s);
	glm::vec3 color = GetResourceColor(s.resourceType);

    RenderMeshColor(meshes[meshName], shaders["TemaShader"], modelMatrix, color);
}

// Deseneaza toate resursele din gari
void Tema2::DrawResources(float deltaTimeSeconds)
{
	// calculez unghiul de rotatie comun pentru toate resursele
    static float rotationAngle = 0.0f;
    float rotationSpeed = 0.8f;
    rotationAngle += rotationSpeed * deltaTimeSeconds;

    for (int i = 0; i < 4; i++) {
        Station* s = stations[i];
        if (s) {
            DrawResource(*s, rotationAngle);
        }
    }
}

// Deseneaza comanda de la Gara Centrala
void Tema2::DrawCentralStationOrder(float deltaTimeSeconds) {
	if (centralStationOrder.empty()) return;

    // calculez pozitia de start
    glm::vec3 startPos = centralStation.resourcePosition;
    
	float spacing = 1.0f; // distanta intre resurse
	float totalWidth = (centralStationOrder.size() - 1) * spacing;

	startPos.x += totalWidth / 2.0f; // centrare orizontala

	// desenez fiecare resursa in parte
    for (size_t i = 0; i < centralStationOrder.size(); i++) {
        ResourceType type = centralStationOrder[i];

		// determin mesh-ul si culoarea
		std::string meshName = GetResourceMesh(type);
		glm::vec3 color = GetResourceColor(type);

		// calculez pozitia curenta
        glm::vec3 currentPos = startPos;
        currentPos.x -= i * spacing;

		// desenez resursa
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, currentPos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f)); // putin mai mici decat resursele din statii

        RenderMeshColor(meshes[meshName], shaders["TemaShader"], modelMatrix, color);
    }

}

// Verifica daca o resursa este in comanda curenta
bool Tema2::IsResourceNeeded(ResourceType res)
{
    if (res == ResourceType::NONE) return false;
	if (centralStationOrder.empty()) return false;

    // verific daca e nevoie de comanda in functie de dificultate
    if (gameDifficulty == GameDifficulty::HARD && centralStationOrder[0] != res) {
        return false;
	}

	// caut resursa in comanda
    for (ResourceType neededRes : centralStationOrder) {
        if (neededRes == res) return true;
    }
    return false;
}

// Colecteaza o resursa si o elimina din comanda
void Tema2::CollectResource(ResourceType res)
{

    // pentru HARD verific ordinea
    if (gameDifficulty == GameDifficulty::HARD) {
        if (!centralStationOrder.empty() && centralStationOrder[0] == res) {

            centralStationOrder.erase(centralStationOrder.begin());

            // adaug in vagonul trenului
            train.collectedResources.push_back(res);
            if (centralStationOrder.empty()) {
                std::cout << "TOATE RESURSELE COLECTATE! Intoarce-te la Gara Centrala pentru livrare!\n";
            }
            else {
                std::cout << "Resursa colectata! Mai ai de adus: " << centralStationOrder.size() << " resurse.\n";
            }

            return;
        }
    }

	// pentru EASY si MEDIUM nu conteaza ordinea
    for (int i = 0; i < centralStationOrder.size(); i++) {

        // daca am gasit resursa cautata
        if (centralStationOrder[i] == res) {
            // o sterg din comanda
            centralStationOrder.erase(centralStationOrder.begin() + i);

			// o adaug in lista de resurse colectate
			train.collectedResources.push_back(res);
            collectedOrnamentsCount++;

			// afisez mesaj in consola
            if (centralStationOrder.empty()) {
                std::cout << "TOATE RESURSELE COLECTATE! Intoarce-te la Gara Centrala pentru livrare!\n";
            }
            else {
                std::cout << "Resursa colectata! Mai ai de adus: " << centralStationOrder.size() << " resurse.\n";
            }

            return;
        }
    }
}

// Actualizeaza statiile (regenerare resurse)
void Tema2::UpdateStations(float deltaTimeSeconds)
{
    for (int i = 0; i < 4; i++) {
        Station* s = stations[i];

		// ignor gara centrala
		if (s->resourceType == ResourceType::NONE) continue;

		// daca resursa e epuizata, decrementam timer-ul de regenerare
        if (!s->hasResource) {
            s->regenerationTimer -= deltaTimeSeconds;
            if (s->regenerationTimer <= 0.0f) {
                s->hasResource = true;
                //cout << "Statia " << i << " a regenerat resursa!\n";
            }
        }
    }
}

// Deseneaza bara de timp ramasa deasupra garii centrale
void Tema2::DrawTimeBar()
{
    // calculez procentul de timp ramas (0.0 la 1.0)
    float ratio = gameTimeRemaining / gameMaxTime;

    // pozitia barei este deasupra garii centrale
    glm::vec3 pos = centralStation.buildingPosition;
	pos.y += 7.0f; // o ridic putin deasupra cladirii

    // desenez fundalul (gri)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 0.5f, 0.2f));

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0.5f));
    }

    // desenez bara efectiva (colorata)
    if (ratio > 0.0f)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);

        // scalez lungimea in functie de timp (scade spre centru)
        // o fac putin mai groasa (0.55) ca sa se vada peste fundal
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f * ratio, 0.55f, 0.25f));

        // calculez culoaera
        glm::vec3 color = glm::mix(TIMEOUT_COLOR, FULLTIME_COLOR, ratio);

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, color);
    }
}

// Deseneaza un X mare deasupra garii centrale cand jocul se termina
void Tema2::DrawGameOverX(float deltaTimeSeconds)
{
    // pozitia deasupra garii centrale
    glm::vec3 pos = centralStation.buildingPosition;
    pos.y += 10.0f; // o ridic mult sa se vada bine

    // animatie de rotatie continua
    static float rotationAngle = 0.0f;
    rotationAngle += 1.0f * deltaTimeSeconds;

    glm::vec3 redColor = glm::vec3(1.0f, 0.0f, 0.0f);

    // bara 1 (diagonala /)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::rotate(modelMatrix, rotationAngle, glm::vec3(0, 1, 0)); // rotatie pe loc
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 0, 1)); // inclinare
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 8.0f, 1.0f));

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, redColor);
    }

    // bara 2 (diagonala \)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, pos);
        modelMatrix = glm::rotate(modelMatrix, rotationAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-45.0f), glm::vec3(0, 0, 1)); // inclinare inversa
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 8.0f, 1.0f));

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, redColor);
    }
}


void Tema2::DrawMiniMap()
{
    // salvez camera si proiectia principala
    implemented::Camera* mainCamera = this->camera;
    glm::mat4 mainProjection = this->projectionMatrix;

    // setez viewport-ul pentru minimap (dreapta-jos)
    glm::ivec2 resolution = window->GetResolution();

    int miniWidth = resolution.x / 4;
    int miniHeight = miniWidth; // patrat
    int miniX = resolution.x - miniWidth - 10;
    int miniY = 10;


	// setez viewport-ul pentru minimap
    glViewport(miniX, miniY, miniWidth, miniHeight);


    // setez proiectia ortografica (cat toata harta)
    float mapSizeX = (float)usedMapCenterX * TILE_SIZE;
    float mapSizeZ = (float)usedMapCenterY * TILE_SIZE;
    this->projectionMatrix = glm::ortho(-mapSizeX, mapSizeX, -mapSizeZ, mapSizeZ, zNear, zFar);

    // activez camera temporara
    this->camera = miniMapCamera;

    // desenez terenul
	DrawTerrain();

    // desenez sinele si garile
    DrawRails();
    DrawStations();

    // desenez trenul simbolic (o sfera mare rosie)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, train.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.0f)); // marita sa se vada bine

        RenderMeshColor(meshes["sphere"], shaders["TemaShader"], modelMatrix, glm::vec3(1, 0, 0));
    }

    // pun inapoi camera si proiectia principala
    this->camera = mainCamera;
    this->projectionMatrix = mainProjection;

    // restaurez viewport-ul initial
    glViewport(0, 0, resolution.x, resolution.y);
}



// Deseneaza bara de timp 2D in coltul ecranului
void Tema2::DrawTimeBar2D()
{
    // salvez starea curenta (camera si proiectia 3d)
    implemented::Camera* mainCamera = this->camera;
    glm::mat4 mainProjection = this->projectionMatrix;

    // setez proiectia 2d => ortografica
    glm::ivec2 resolution = window->GetResolution();
    this->projectionMatrix = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, zNear, zFar);

    // activez camera temporara
    this->camera = timeCamera;

    // desenez bara de timp in coltul din stanga-sus
	float barWidth = resolution.x / 5.0f;
	float barHeight = resolution.y / 30.0f;
	float marginX = resolution.x / 30.0f;
	float marginY = resolution.y / 25.0f;

    // calcul pozitie y (sus)
    float yPos = (float)resolution.y - marginY - barHeight / 2.0f;

    // desenez fundalul (gri)
    {
        float xCenter = marginX + barWidth / 2.0f;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xCenter, yPos, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(barWidth, barHeight, 1));

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, glm::vec3(0.5f));
    }

    // desenez timpul ramas (colorat)
    {
        float ratio = gameTimeRemaining / gameMaxTime;
        if (ratio < 0) ratio = 0;

        float currentWidth = barWidth * ratio;
        float xCenter = marginX + currentWidth / 2.0f;

        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(xCenter, yPos, 1.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(currentWidth, barHeight, 1));


        glm::vec3 color = glm::mix(TIMEOUT_COLOR, FULLTIME_COLOR, ratio);

        RenderMeshColor(meshes["box"], shaders["TemaShader"], modelMatrix, color);
    }

    // pun inapoi setarile 3d
    this->camera = mainCamera;
    this->projectionMatrix = mainProjection;
}

// Reseteaza jocul la starea initiala
void Tema2::ResetGame()
{
    // resetez starea jocului
    gameState = GameState::RUNNING;
    gameTimeRemaining = gameMaxTime;
	totalGameTime = 0.0f;

	collectedOrnamentsCount = 0;
	treeOrnaments.clear();

    // resetez trenul
    InitTrain();

    // resetez statiile
    for (int i = 0; i < 4; i++) {
        Station* s = stations[i];
        if (s) {
            s->hasResource = true;
            s->regenerationTimer = RESOURCE_REGEN_TIME;
        }
    }
    // resetez comanda de la gara centrala
    centralStationOrder.clear();
    GenerateOrder();

	StartGame();
}

// Deseneaza un brad de craciun langa gara centrala
void Tema2::DrawChristmasTree(float deltaTimeSeconds)
{
    // calculez pozitia bradului langa gara centrala
    glm::vec3 treePos = centralStation.centerPosition;
    treePos.x += 7.0f; // il mut putin la dreapta
	treePos.z += 5.0f; // il mut putin in spate
    treePos.y = 0;     // il fixez pe pamant

    // desenez trunchiul (cilindru maro)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, treePos + glm::vec3(0, TREE_TRUNK_HEIGHT / 2.0f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(TREE_TRUNK_RADIUS, TREE_TRUNK_HEIGHT, TREE_TRUNK_RADIUS));
        RenderMeshColor(meshes["cylinder"], shaders["TemaShader"], modelMatrix, TREE_TRUNK_COLOR);
    }

    float currentY = TREE_TRUNK_HEIGHT - 0.5f; // deasupra trunchiului cu putin overlap
    float centerY;
    // Nivel 1 (Jos)
    {
		centerY = currentY + TREE_LEVEL_HEIGHT / 2.0f;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, treePos + glm::vec3(0, centerY, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(TREE_L1_SCALE, TREE_LEVEL_HEIGHT, TREE_L1_SCALE));
        RenderMeshColor(meshes["cone"], shaders["TemaShader"], modelMatrix, TREE_BODY_COLOR);
    }

	currentY += TREE_LEVEL_HEIGHT - TREE_LEVEL_HEIGHT * 0.3f; // overlap intre nivele

    // Nivel 2 (Mijloc)
    {
		centerY = currentY + TREE_LEVEL_HEIGHT / 2.0f;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, treePos + glm::vec3(0, centerY, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(TREE_L2_SCALE, TREE_LEVEL_HEIGHT, TREE_L2_SCALE));
        RenderMeshColor(meshes["cone"], shaders["TemaShader"], modelMatrix, TREE_BODY_COLOR);
    }

    currentY += TREE_LEVEL_HEIGHT - TREE_LEVEL_HEIGHT * 0.4f;

    // Nivel 3 (Varf)
    {
		centerY = currentY + TREE_LEVEL_HEIGHT / 2.0f;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, treePos + glm::vec3(0, centerY, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(TREE_L3_SCALE, TREE_LEVEL_HEIGHT - 0.5, TREE_L3_SCALE));
        RenderMeshColor(meshes["cone"], shaders["TemaShader"], modelMatrix, TREE_BODY_COLOR);
    }

	// desenez ornamentele in brad
    for (const auto& ornament : treeOrnaments) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, treePos + ornament.relativePos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));     // dimensiune glob

        std::string mName = GetResourceMesh(ornament.type);
        glm::vec3 mColor = GetResourceColor(ornament.type);

        RenderMeshColor(meshes[mName], shaders["TemaShader"], modelMatrix, mColor);
    }
}

void Tema2::PrintChristmasTree() {
    cout << R"(

                       |
                      -+-
                       A
                      /=\               /\  /\    ___  _ __  _ __ __    __
                    i/ O \i            /  \/  \  / _ \| '__|| '__|\ \  / /
                    /=====\           / /\  /\ \|  __/| |   | |    \ \/ /
                    /  i  \           \ \ \/ / / \___/|_|   |_|     \  /
                  i/ O * O \i                                       / /
                  /=========\        __  __                        /_/    _
                  /  *   *  \        \ \/ /        /\  /\    __ _  ____  | |
                i/ O   i   O \i       \  /   __   /  \/  \  / _` |/ ___\ |_|
                /=============\       /  \  |__| / /\  /\ \| (_| |\___ \  _
                /  O   i   O  \      /_/\_\      \ \ \/ / / \__,_|\____/ |_|
              i/ *   O   O   * \i
              /=================\
                     |___|

)" << endl;
}

// Afiseaza povestea introductiva a jocului
void Tema2::PrintIntroStory()
{
	// golesc consola
    system("cls");
    PrintChristmasTree();

    cout << "          ===========================================================" << endl;
    cout << "                     CRACIUNUL IN WHOVILLE (Lumea lui Grinch)        " << endl;
    cout << "          ===========================================================" << endl;
    cout << endl;
    cout << "             Oh, nu! Grinch a incercat sa fure Craciunul din nou!" << endl;
    cout << "             A daramat bradul din Piata Centrala si a imprastiat" << endl;
    cout << "             toate ornamentele prin gari!" << endl;
    cout << endl;
    cout << "             Misiunea ta, ca Mecanic Sef al Whoville Express, este:" << endl;
    cout << "             1. Mergi la gari si recupereaza ornamentele cerute" << endl;
    cout << "             2. Adu-le la Gara Centrala pentru a impodobi bradul" << endl;
    cout << "             3. Grabeste-te! Craciunul vine in curand!" << endl;
    cout << endl;
    cout << "         >>> APASA [W] PENTRU A PORNI LOCOMOTIVA SI A SALVA CRACIUNUL! <<<" << endl;
    cout << "         ===============================================================" << endl << endl << endl;
}

// Afiseaza statisticile jocului
void Tema2::PrintOutroStory()
{
    system("cls");
    PrintChristmasTree();

    if (collectedOrnamentsCount == 0) {
        cout << "          ========================================================================" << endl;
        cout << "                           OH NU! CRACIUNUL A FOST FURAT...                       " << endl;
        cout << "          ========================================================================" << endl;
        cout << endl;
        cout << "             Din pacate, nu ai reusit sa aduci" << endl;
        cout << "             niciun ornament la timp..." << endl;
        cout << endl;
        cout << "             Bradul a ramas gol si trist." << endl;
        cout << "             Grinch a castigat de data asta :(" << endl;
        cout << endl;
	}
    else
    {
        cout << endl;
        cout << "         ===========================================================" << endl;
        cout << "                             CRACIUN SALVAT!                           " << endl;
        cout << "         ===========================================================" << endl;
        cout << endl;
        cout << "             Incredibil! Ai reusit sa impodobesti bradul la timp!" << endl;
        cout << "             Inima lui Grinch a crescut de 3 ori in aceasta zi," << endl;
        cout << "             vazand cat de frumos arata bradul facut de tine! <3" << endl;
        cout << endl;
        cout << "                        --- STATISTICI MECANIC ---" << endl;
        cout << "             Timp Total Jucat: " << (int)totalGameTime << " secunde" << endl;
        cout << "             Ornamente Livrate: " << collectedOrnamentsCount << " bucati" << endl;
    }
   
	// Afisez scorul final
	totalScore += collectedOrnamentsCount * 100;
    cout << "             SCOR FINAL: " << totalScore << " puncte Whoville" << endl;
    cout << endl;
    cout << "             >>> APASA [ENTER] PENTRU A JUCA DIN NOU <<<" << endl;
    cout << "         ===========================================================" << endl << endl << endl;
}

// Porneste jocul
void Tema2::StartGame()
{
    gameState = GameState::RUNNING;

    // resetez variabilele
    collectedOrnamentsCount = 0;
	train.collectedResources.clear();
	treeOrnaments.clear();
    totalGameTime = 0.0f;
    gameTimeRemaining = gameMaxTime;
	totalScore = 0;

    PrintIntroStory();
}

void Tema2::EndGame()
{
    gameState = GameState::GAME_OVER;
    PrintOutroStory();
}

void Tema2::Init()
{
    gameTimeRemaining = gameMaxTime;

	InitMountain();
	InitRiver();

    InitTerrain();
    InitTerrainMesh();
    InitCylinderMesh();
	InitConeMesh();
    InitPyramidMesh();
	InitHexPyramidMesh();
	InitHexPrismMesh();

	InitRails();

	InitTrain();

    StartGame();

    renderCameraTarget = false;

    camera = new implemented::Camera();

    //camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));


    glm::vec3 center(0.0f, 0.0f, 0.0f);
    camera->Set(center + glm::vec3(0, 5, 0.01f), center, glm::vec3(0, 1, 0));


    // Initializez camerele suplimentare
    miniMapCamera = new implemented::Camera();
    miniMapCamera->Set(glm::vec3(0, 50, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

    timeCamera = new implemented::Camera();
    timeCamera->Set(glm::vec3(0, 0, 50), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));



    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }



    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}




void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    //glClearColor(0, 0, 0, 1);

    if (GameState::GAME_OVER == gameState)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // fundal inchis
	else
	    glClearColor(0.53, 0.96, 0.96, 1.0f);    // cer albastru

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}



void Tema2::Update(float deltaTimeSeconds)
{
    totalGameTime += deltaTimeSeconds;

    if (isCameraLocked) {

		// Actualizez pozitia camerei pentru a urmari trenul
		glm::vec3 targetPosition = train.position + glm::vec3(0, 4.0f, 0); // putin deasupra trenului


        camera->position = targetPosition - glm::normalize(camera->forward) * camera->distanceToTarget;
	}

    if (gameState == GameState::GAME_OVER) {
        gameTimeRemaining = 0.0f;

		// desenez scena de joc in continuare, dar fara update-uri
		DrawTerrain();
        DrawRails();
        DrawStations();
        DrawTrain();
        DrawChristmasTree(deltaTimeSeconds);

		DrawGameOverX(deltaTimeSeconds);

		return;
    }


	DrawTimeBar();

	// Actualizez timpul ramas
    gameTimeRemaining -= deltaTimeSeconds;
    
	// Verific daca timpul a expirat
    if (gameTimeRemaining < 0.0f) {
        gameTimeRemaining = 0.0f;
        EndGame();
    }

    UpdateStations(deltaTimeSeconds);

    MoveTrain(deltaTimeSeconds);

    DrawTerrain();
	DrawRails();
    DrawStations();
    DrawChristmasTree(deltaTimeSeconds);
	DrawResources(deltaTimeSeconds);

	DrawTrain();
	DrawHook();

    DrawCentralStationOrder(deltaTimeSeconds);

    DrawTimeBar2D();
    DrawMiniMap();

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix);
    }
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int locUse = glGetUniformLocation(shader->program, "use_object_color");
    if (locUse >= 0) glUniform1i(locUse, 0);

    int locSensitive = glGetUniformLocation(shaders["TemaShader"]->program, "is_time_sensitive");
    if (locSensitive >= 0) glUniform1i(locSensitive, 0);


    mesh->Render();
}


void Tema2::RenderMeshColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int locUse = glGetUniformLocation(shader->program, "use_object_color");
    if (locUse >= 0) glUniform1i(locUse, 1);

    int locColor = glGetUniformLocation(shader->program, "object_color");
    if (locColor >= 0) glUniform3fv(locColor, 1, glm::value_ptr(color));

    mesh->Render();
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Change projection parameters. Declare any extra
 // variables you might need in the class header. Inspect this file
 // for any hardcoded projection arguments (can you find any?) and
 // replace them with those extra variables.
    if (isPerspective)
    {
        if (window->KeyHold(GLFW_KEY_U))
        {
            fov += deltaTime;
            projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_I))
        {
            fov -= deltaTime;
            projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
        }
    }

    if (!isPerspective)
    {
        if (window->KeyHold(GLFW_KEY_1))
        {
            left += deltaTime;
            right -= deltaTime;

        }

        if (window->KeyHold(GLFW_KEY_2))
        {
            left -= deltaTime;
            right += deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_3))
        {
            top -= deltaTime;
            bottom += deltaTime;

        }

        if (window->KeyHold(GLFW_KEY_4))
        {
            top += deltaTime;
            bottom -= deltaTime;
        }

        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT && !isCameraLocked))
    {
        float cameraSpeed = 9.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            //camera->TranslateForward(deltaTime * cameraSpeed);
            //camera->MoveForward(deltaTime * cameraSpeed);
            camera->MoveForward(deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->MoveForward(-deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-deltaTime * cameraSpeed);

        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(deltaTime * cameraSpeed);

        }
    } 
    else if (train.state == TrainState::WAITING_AT_INTERSECTION || train.state == TrainState::WAITING_START || train.state == TrainState::WAITING_IN_STATION) {
        int selectedRail = -1;

        if (window->KeyHold(GLFW_KEY_W)) selectedRail = FindNextRail('W');
        else if (window->KeyHold(GLFW_KEY_A)) selectedRail = FindNextRail('A');
        else if (window->KeyHold(GLFW_KEY_D)) selectedRail = FindNextRail('D');
        else if (window->KeyHold(GLFW_KEY_S)) selectedRail = FindNextRail('S');


        if (selectedRail != -1)
        {
            train.currentRail = selectedRail;
            train.progress = 0.0f;
            train.state = TrainState::MOVING;
            train.possibleNextRails.clear();

			// recalculez directia trenului
            const Rail& r = railRoad[train.currentRail];
            train.direction = glm::normalize(r.endPosition - r.startPosition);
        }
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O)
    {
        // proiectie ortografica
        isPerspective = false;
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    if (key == GLFW_KEY_P)
    {
        // proiectie perspectiva
        isPerspective = true;
        projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
    }

    if (key == GLFW_KEY_SPACE) {
        isCameraLocked = !isCameraLocked;
    }

	// Daca apasa enter cand e game over, reseteaza jocul
    if (key == GLFW_KEY_ENTER && gameState == GameState::GAME_OVER) {
        ResetGame();
	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
			camera->RotateFirstPerson_OX(-deltaY * sensivityOY);

			camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
			camera->RotateThirdPerson_OX(-deltaY * sensivityOY);

			camera->RotateThirdPerson_OY(-deltaX * sensivityOX);

        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
