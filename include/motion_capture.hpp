#pragma once

#include <opencv2/opencv.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>

// Estructura para los datos de los marcadores detectados (no se usa directamente en getMarkerPositions ahora)
// struct DetectedMarker {
//     int id; // Podría ser el índice en el vector para color tracking
//     sf::Vector2f screenPosition;
// };

class MotionCapture {
public:
    MotionCapture();
    ~MotionCapture();

    bool initializeCamera(int cameraIndex = 0);
    void releaseCamera();
    bool isCameraOpen() const;

    // Devuelve un vector de posiciones 2D de los objetos azules detectados.
    // El orden en el vector puede depender de cómo OpenCV encuentre los contornos.
    std::vector<sf::Vector2f> getBlueObjectPositions();

    // Para mostrar la ventana de OpenCV con la máscara de color (opcional, para debug)
    void displayDebugCameraView(bool showMask = false);
    cv::Mat getCurrentFrame(); // Para obtener el frame si necesitas procesarlo fuera

    // Rangos HSV para el color azul (puedes ajustarlos)
    // Estos son solo ejemplos, necesitarás afinarlos.
    // Para azul: H: 100-130, S: 150-255, V: 50-255 (aproximado)
    int blue_H_min = 100, blue_H_max = 130;
    int blue_S_min = 100, blue_S_max = 255; // Aumentado el mínimo de S para colores más saturados
    int blue_V_min = 100, blue_V_max = 255; // Aumentado el mínimo de V para colores más brillantes

    // Mínima área de contorno para ser considerado un objeto
    double minContourArea = 100.0;


private:
    cv::VideoCapture cap;
    cv::Mat hsvFrame;
    cv::Mat colorMask;
    // Otros miembros necesarios para OpenCV
};