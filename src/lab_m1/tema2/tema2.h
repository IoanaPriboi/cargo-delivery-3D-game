#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema2_camera.h"


namespace m1
{
    /* ====================== CONFIG GLOBAL ====================== */

    // --- Dimensiuni Harta ---
    //const int MAP_W = 54;
    //const int MAP_H = 38;

    const int MAP_W = 201;
    const int MAP_H = 201;

    const int USED_MAP_W = 96;
    const int USED_MAP_H = 80;

    const float TILE_SIZE = 1.0f;

    // --- Culori Mediu ---
	const glm::vec3 GRASS_COLOR = glm::vec3(0.1f, 0.8f, 0.1f);
	const glm::vec3 WATER_COLOR = glm::vec3(0.2f, 0.5f, 0.9f);
	const glm::vec3 MOUNTAIN_COLOR = glm::vec3(0.55f, 0.35f, 0.2f);
    const glm::vec3 SKY_COLOR = glm::vec3(0.53f, 0.81f, 0.92f);
    const glm::vec3 TIMEOUT_COLOR = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 FULLTIME_COLOR = glm::vec3(0.0f, 1.0f, 0.0f);

	// ==================== RAIL CONFIG ====================
	const float RAIL_WIDTH = 1.2f;
	const float RAIL_HEIGHT = 0.3f;

    // --- Culori Sine ---
    const glm::vec3 NORMAL_RAIL_COLOR = glm::vec3(0.0f, 0.0f, 0.0f);
    const glm::vec3 BRIDGE_RAIL_COLOR = glm::vec3(0.60f, 0.60f, 0.60f);
    const glm::vec3 TUNNEL_RAIL_COLOR = glm::vec3(0.4f, 0.2f, 0.1f);
    const glm::vec3 STATION_RAIL_COLOR = glm::vec3(0.8f, 0.8f, 0.0f);
    


	// ==================== TRAIN CONFIG ====================

    // --- Culori Tren ---
    const glm::vec3 TRAIN_COLOR_WHEEL = glm::vec3(0.9f, 0.1f, 0.1f);    // Rosu
    const glm::vec3 TRAIN_COLOR_CHASSIS = glm::vec3(1.0f, 0.9f, 0.0f);  // Galben
    const glm::vec3 TRAIN_COLOR_BODY = glm::vec3(0.3f, 0.9f, 0.3f);     // Verde (vagon + cabina)
    const glm::vec3 TRAIN_COLOR_ENGINE = glm::vec3(0.2f, 0.2f, 0.9f);   // Albastru
    const glm::vec3 TRAIN_COLOR_DETAIL = glm::vec3(1.0f, 0.4f, 0.7f);   // Roz

    // --- Dimensiuni Tren ---
    // Roti
    const float WHEEL_RADIUS = 0.15f;
    const float WHEEL_WIDTH = 0.1f;

    // Sasiu (placa galbena dintre roti si corp)
    const float CHASSIS_HEIGHT = 0.1f;

    // Locomotiva
    const float LOCOMOTIVE_LENGTH = 1.5f;
    const float LOCOMOTIVE_WIDTH = 1.0f;
    const float CABIN_HEIGHT = 0.9f;
    const float ENGINE_DIAMETER = 0.6f;

    // Proportii lungime locomotiva
	const float LOCO_CABIN_RATIO = 0.4f;   // cat din locomotiva e cabina vatmanului

    // Detaliu motor (cilindru roz)
    const float DETAIL_RADIUS = 0.1f;
    const float DETAIL_LENGTH = 0.1f;

    // Vagon
    const float WAGON_LENGTH = 2.8f;
    const float WAGON_WIDTH = 0.9f;
    const float WAGON_HEIGHT = 0.8f;
    const float COUPLING_DISTANCE = 0.2f; // Spatiu intre locomotiva si vagon

    // Carlig
    const float HOOK_SIZE = 0.2f;
    const glm::vec3 HOOK_OFFSET = glm::vec3(0.0f, 1.0f, 0.0f);


	// ==================== STATION CONFIG ====================
    // --- Culori Gari ---
    const glm::vec3 CENTRAL_STATION_COLOR = glm::vec3(0.65, 0.98, 0.68);    // verde f deschis
	const glm::vec3 RESOURCE_1_COLOR = glm::vec3(0.45, 0.13, 0.76);         // mov
	const glm::vec3 RESOURCE_2_COLOR = glm::vec3(0.21, 0.22, 0.65);         // albastru
	const glm::vec3 RESOURCE_3_COLOR = glm::vec3(0.73, 0.72, 0.08);         // galben

    // --- Pozitii Gari ---
	const glm::ivec2 CENTRAL_STATION_POS_START = glm::vec2(-4, -7);
    const glm::ivec2 CENTRAL_STATION_POS_END = glm::vec2(0, -7);

	const glm::ivec2 RESOURCE_STATION_1_POS_START = glm::vec2(-22, -26);
	const glm::ivec2 RESOURCE_STATION_1_POS_END = glm::vec2(-18, -26);

	const glm::ivec2 RESOURCE_STATION_2_POS_START = glm::vec2(44, -23);
	const glm::ivec2 RESOURCE_STATION_2_POS_END = glm::vec2(44, -27);

	const glm::ivec2 RESOURCE_STATION_3_POS_START = glm::vec2(21, 17);
	const glm::ivec2 RESOURCE_STATION_3_POS_END = glm::vec2(25, 17);

    // --- Dimensiuni Gari ---
	const float RESOURCE_STATION_SIZE = 4.0f;
	const float CENTRAL_STATION_SIZE = 8.0f;

	const float RESOURCE_STATION_HEIGHT = 5.0f;
	const float CENTRAL_STATION_HEIGHT = 5.0f;


    // ==================== RESOURCE CONFIG ====================
    // --- Timp ---
    const float RESOURCE_LOAD_TIME = 2.0f;  // cat dureaza incarcarea resurselor
    const float RESOURCE_REGEN_TIME = 5.0f; // cat dureaza regenerarea resurselor

    // --- Dimensiuni ---
	const float RESOURCE_SIZE = 0.7f;
	const glm::vec3 RESOURCE_OFFSET = glm::vec3(2.0f, 1.5f, 0); // offset resursa fata de statie
	const float RESOURCE_OFFSET_HEIGHT = 1.5f; // cat sta deasupra solului

    // ==================== TREE CONFIG ====================
	const float TREE_LEVEL_HEIGHT = 3.0f; // inaltimea fiecarui nivel din brad

    // Scalarea (latimea) pentru fiecare etaj
    const float TREE_L1_SCALE = 3.5f;
    const float TREE_L2_SCALE = 2.5f;
    const float TREE_L3_SCALE = 1.5f;

	// Dimendiuni trunchi
	const float TREE_TRUNK_HEIGHT = 1.6f;
	const float TREE_TRUNK_RADIUS = 0.7f;

	const glm::vec3 TREE_BODY_COLOR = glm::vec3(0.0f, 0.5f, 0.0f);
	const glm::vec3 TREE_TRUNK_COLOR = glm::vec3(0.4f, 0.2f, 0.1f);
	
	// ==================== GAME CONFIG ====================
	const float GAME_TIME_EASY = 100.0f;    // secunde
	const float GAME_TIME_MEDIUM = 60.0f;   // secunde
	const float GAME_TIME_HARD = 200.0f;     // secunde
	const float SCORE_PER_SECOND = 55.0f;    // puncte pe secunda ramasa


    // =================== ENUMS (STARI) =================
    enum GameState {
        RUNNING,
        GAME_OVER,
	};

    enum GameDifficulty {
        EASY,
        MEDIUM,
        HARD
    };

    enum TerrainType {
        GRASS,
        WATER,
		MOUNTAIN
	};

    enum RailType {
        NORMAL,
        BRIDGE,
		TUNNEL, 
        STATION
	};

    enum ResourceType {
        NONE,
        RESOURCE_1,     // cub
        RESOURCE_2,     // sfera
        RESOURCE_3,     // piramida
    };

    enum HookState {
        IDLE,       // ascuns
        EXTENDING,  // iese spre resursa
        RETRACTING  // se intoarce la tren
    };

    enum TrainState {
        MOVING,
        WAITING_START,
        WAITING_AT_INTERSECTION,
        WAITING_IN_STATION,
        LOADING_RESOURCE
    };

    struct Mountain {
        int xMin;
		int yMin;

		int xMax;
		int yMax;

		int length;
		int height;
    };

    struct WaterSegment {
        int xMin;
        int yMin;

        int xMax;
        int yMax;

        int length;
        int height;
    };

	struct River {
		int numSegments = 4;
        WaterSegment segments[5];
	};


    struct Station {
		glm::vec3 startPosition;
		glm::vec3 endPosition;
		glm::vec3 centerPosition;

		int forwardRailId = -1;
		int backwardRailId = -1;

		glm::vec3 buildingPosition;
		int buildingRailId = -1;

		ResourceType resourceType = NONE;
		bool hasResource = true;
		float regenerationTimer = 0.0f; // cronometru pentru regenerare
		glm::vec3 resourcePosition;

	};

    struct Rail {
        int id;
		RailType type;

		glm::vec3 startPosition;
		glm::vec3 endPosition;
        glm::vec3 centerPosition;

		float length;

		// vector de indici catre urmatoarele segmente de sine
		std::vector<int> nextRails;
    };


    struct Train {
		int currentRail = 0;
		float progress = 0.0f;  // [0, 1]
		float speed = 9.0f;
		float waitTimer = 0.0f;

		glm::vec3 position;
		glm::vec3 direction = glm::vec3(1, 0, 0);


		TrainState state = WAITING_START;
		std::vector<int> possibleNextRails; // indici catre segmentele posibile la intersectii

		// variabile pentru colectarea resurselor
        HookState hookState = IDLE;
        glm::vec3 hookTipPosition;      // pozitia curenta a varfului carligului
        glm::vec3 hookTargetPosition;   // unde este resursa (pentru calcularea directiei)
        float hookSpeed = 4.0f;         // viteza de miscare a carligului

        // lista de resurse colectate de tren
        std::vector<ResourceType> collectedResources;
	};


    struct BoundingBox {
        glm::vec3 min;
        glm::vec3 max;

		// constructor cu parametri
        BoundingBox(glm::vec3 center, glm::vec3 size) {
            min = center - size * 0.5f;
            max = center + size * 0.5f;
        }
    };

    struct Ornament {
        ResourceType type;
        glm::vec3 relativePos;
    };

    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void RenderMeshColor(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    
	    // ====================== INITIALIZARI =========================
        // --- Harta ---
        void InitMountain();
        void InitRiver();
        void InitTerrain();      // populeaza matricea terrain cu tipuri de sol

        // --- Obiecte joc ---
        void InitCentralStation();
        void InitResourceStations();
        void InitRails();        // construieste toata reteaua de sine
        void InitTrain();        // plaseaza trenul la start



        // ====================== MESH-uri ============================
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void InitTerrainMesh();
        void InitCylinderMesh();
        void InitConeMesh();
        void InitPyramidMesh();
        void InitHexPyramidMesh();
        void InitHexPrismMesh();


	    // ====================== SINE ============================

        // adauga un segment simplu, unidirectional (a -> b)
        void AddRail(const glm::ivec2& a, const glm::ivec2& b, RailType type);

        // adauga un segment bidirectional (a <-> b)
        void AddBiRail(const glm::ivec2& a, const glm::ivec2& b, RailType type);

        // adauga sine bidirectionale, impartind automat segmentul in functie de teren (pod/tunel/normal)
        void AddBiRailSplitByTerrain(const glm::ivec2& a, const glm::ivec2& b);

	    // calculeaza conexiunile dintre sine (nextRails)
        void BuildRailConnections();


        // ===========================  MISCARE =====================================

        void MoveTrain(float deltaTimeSeconds);   // actualizeaza pozitia si starea trenului
        void UpdateStations(float deltaTime);     // gestioneaza timerele de regenerare resurse

        // --- Comenzi ---
        void GenerateOrder();                     // genereaza o noua lista de cerinte la gara centrala
        bool IsResourceNeeded(ResourceType res);  // verifica daca o resursa colectata este ceruta
        void CollectResource(ResourceType res);   // proceseaza colectarea (sterge din lista, update scor)

        // --- Control ---
        // returneaza id-ul sinei urmatoare bazat pe input (WASD) sau -1 daca nu exista
        int FindNextRail(char command);

		void ResetGame(); // reseteaza jocul


        // ======================== DESENARE ====================================

        // --- Mediu ---
        void DrawTerrain();

        // --- Sine ---
        void DrawRails();       // deseneaza toate sinele
        void DrawNormalRail(const Rail& rail);
        void DrawBridgeRail(const Rail& rail);
        void DrawTunnelRail(const Rail& rail);
        void DrawStationRail(const Rail& rail);

        // --- Statii si Resurse ---
        void DrawStations();      // deseneaza cladirile garilor
        void DrawStationBuilding(const Station& station);
		void DrawResources(float deltaTimeSeconds);
        void DrawResource(const Station& station, float rotationAngle);

        // --- Tren  ---
        void DrawTrain();
        void DrawHook();          // deseneaza carligul trenului

        // --- Interfata ---
        void DrawTimeBar2D();
        void DrawTimeBar();
	    void DrawCentralStationOrder(float deltaTimeSeconds);
        void DrawGameOverX(float deltaTimeSeconds);
        void DrawMiniMap();
		void DrawChristmasTree(float deltaTimeSeconds);

        void PrintIntroStory();
        void PrintOutroStory();
        void StartGame();
        void EndGame();
        void PrintChristmasTree();

        // --- Debug ---
        void PrintRailsInfo();    // afiseaza detalii despre sine in consola


        // ====================== HELPERE ================================

        // --- Conversii Coordonate ---
        bool MapPosToIndex(int mapX, int mapY, int& indexX, int& indexY);
        glm::vec3 MapPosToWorldPos(int mapX, int mapY);

        // --- Harta ---
        void FillRectMap(int xMin, int yMin, int xMax, int yMax, TerrainType type);
        bool GetTerrainAt(int mapX, int mapY, TerrainType& outType);
        TerrainType GetTerrainAtWorldPos(const glm::vec3& worldPos);
        glm::vec3 GetTerrainColor(TerrainType type);

        // --- Sine ---
        RailType GetRailTypeFromTerrain(TerrainType t);
        RailType GetRailTypeForSegment(const glm::ivec2& posA, const glm::ivec2& posB);     // determina tipul sinei necesar intre doua puncte
    
        glm::mat4 ComputeRailModelMatrix(const Rail& rail);
        glm::vec3 GetRailColor(RailType type);

        // --- Statii si Gari ---
        std::string GetMeshNameForStation(const Station& station);
        glm::vec3 GetStationRailColor(const Station& station);
        Station* GetStationByRailId(int railId);
        float GetStationBuildingSize(const Station& station);
        float GetStationBuildingHeight(const Station& station);
        void SetupStationBuilding(Station& station);            // calculeaza pozitia garii fata de sina
        glm::vec3 GetResourceColor(ResourceType rt);
        std::string GetResourceMesh(ResourceType rt);

        // --- Coliziuni ---
        bool CheckCollision(const BoundingBox& a, const BoundingBox& b);    


     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool isCameraLocked = true;
        bool isPerspective = true;

		// camere suplimentare pt mini map si timp
        implemented::Camera* miniMapCamera;
        implemented::Camera* timeCamera;

        int mapCenterX = MAP_W / 2;
        int mapCenterY = MAP_H / 2;

		int usedMapCenterX = USED_MAP_W / 2;
        int usedMapCenterY = USED_MAP_H / 2;


        // proectie perspectiva
        float fov = RADIANS(60);
        float zNear = 0.01f;
        float zFar = 300.0f;

        // proiectie ortografica
        float left = -mapCenterX * TILE_SIZE;
        float right = mapCenterX * TILE_SIZE;
        float bottom = -mapCenterX * TILE_SIZE;
        float top = mapCenterX * TILE_SIZE;

		GameState gameState = RUNNING;

		Mountain mountain;
		River river;

        TerrainType terrain[MAP_W][MAP_H];

        std::vector<Rail> railRoad;
		Train train;


        Station centralStation; // Gara Centrala
        Station resourceStation1;
        Station resourceStation2;
        Station resourceStation3;

        Station* stations[4] = { &centralStation,
                              &resourceStation1,
                              &resourceStation2,
                              &resourceStation3
		};


		// lista de resurse cerute la Gara Centrala
		std::vector<ResourceType> centralStationOrder;
        std::vector<Ornament> treeOrnaments;

        // Game specs
        float gameMaxTime;
        float gameTimeRemaining;
		GameDifficulty gameDifficulty = HARD;

		float totalGameTime = 0.0f; // timpul total petrecut in joc
		float totalScore = 0.0f;
        int collectedOrnamentsCount = 0;

    };
}   // namespace m1
