#include "motion_capture.hpp"
#include <iostream>
#include <algorithm> // Para std::sort (opcional, si quieres ordenar contornos)

MotionCapture::MotionCapture() {
    // Constructor
}

MotionCapture::~MotionCapture() {
    releaseCamera();
}

bool MotionCapture::initializeCamera(int cameraIndex) {
    cap.open(cameraIndex);
    if (!cap.isOpened()) {
        std::cerr << "ERROR: No se pudo abrir la cámara con índice " << cameraIndex << std::endl;
        return false;
    }
    std::cout << "Cámara inicializada correctamente." << std::endl;
    return true;
}

void MotionCapture::releaseCamera() {
    if (cap.isOpened()) {
        cap.release();
        std::cout << "Cámara liberada." << std::endl;
    }
}

bool MotionCapture::isCameraOpen() const {
    return cap.isOpened();
}

cv::Mat MotionCapture::getCurrentFrame() {
    cv::Mat frame;
    if (cap.isOpened()) {
        cap >> frame;
        if (!frame.empty()) {
             cv::flip(frame, frame, 1); // Espejar horizontalmente
        }
    }
    return frame;
}

std::vector<sf::Vector2f> MotionCapture::getBlueObjectPositions() {
    std::vector<sf::Vector2f> positions;
    if (!isCameraOpen()) {
        return positions;
    }

    cv::Mat frame = getCurrentFrame();
    if (frame.empty()) {
        std::cerr << "ERROR: Frame vacío capturado en getBlueObjectPositions." << std::endl;
        return positions;
    }

    // Convertir frame a HSV
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

    // Crear máscara para el color azul
    cv::Scalar lowerBlue = cv::Scalar(blue_H_min, blue_S_min, blue_V_min);
    cv::Scalar upperBlue = cv::Scalar(blue_H_max, blue_S_max, blue_V_max);
    cv::inRange(hsvFrame, lowerBlue, upperBlue, colorMask);

    // Operaciones morfológicas para limpiar la máscara
    // Erosión para eliminar pequeños puntos de ruido
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::erode(colorMask, colorMask, kernel, cv::Point(-1,-1), 1); // 1 iteración de erosión
    // Dilatación para cerrar huecos y restaurar tamaño de objetos
    cv::dilate(colorMask, colorMask, kernel, cv::Point(-1,-1), 2); // 2 iteraciones de dilatación


    // Encontrar contornos
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(colorMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Opcional: Ordenar contornos (ej. de izquierda a derecha, o por área)
    // std::sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2){
    //     cv::Moments m1 = cv::moments(c1);
    //     cv::Moments m2 = cv::moments(c2);
    //     return m1.m10 / m1.m00 < m2.m10 / m2.m00; // Ordenar por coordenada X del centroide
    // });


    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > minContourArea) { // Filtrar por área mínima
            cv::Moments m = cv::moments(contour);
            if (m.m00 > 0) { // Evitar división por cero
                cv::Point2f center(static_cast<float>(m.m10 / m.m00), static_cast<float>(m.m01 / m.m00));
                positions.push_back(sf::Vector2f(center.x, center.y));
            }
        }
    }
    return positions;
}

void MotionCapture::displayDebugCameraView(bool showMask) {
    if (!isCameraOpen()) return;

    cv::Mat frame = getCurrentFrame(); // Obtiene el frame ya espejado
    if (frame.empty()) return;

    if (showMask) {
        // Asegurarse de que colorMask esté actualizada (se actualiza en getBlueObjectPositions)
        // Para esta demo, si getBlueObjectPositions no se llama en el mismo ciclo,
        // la máscara podría no estar lista o ser de un frame anterior.
        // Sería mejor recalcularla aquí si es necesario o asegurar el flujo.
        // Por simplicidad, asumimos que se llamó getBlueObjectPositions antes en el bucle principal.
        if (!colorMask.empty()) {
            cv::Mat maskedFrame;
            cv::bitwise_and(frame, frame, maskedFrame, colorMask);
            // cv::imshow("OpenCV Debug View (Masked)", maskedFrame);
            // cv::imshow("Color Mask", colorMask); // Para ver solo la máscara binaria
        } else {
            //  cv::imshow("OpenCV Debug View (Original)", frame); // Mostrar original si no hay máscara
        }
    } else {
        // Dibujar círculos en los puntos detectados si no se muestra la máscara
        // (requeriría llamar a getBlueObjectPositions y dibujar sobre 'frame')
        // O simplemente mostrar el frame original
        std::vector<sf::Vector2f> bluePoints = getBlueObjectPositions(); // Llama para actualizar y obtener puntos
        cv::Mat displayFrame = frame.clone(); // Clonar para no modificar el frame original que podría usarse en otro lado
        for(const auto& point : bluePoints){
            cv::circle(displayFrame, cv::Point(static_cast<int>(point.x), static_cast<int>(point.y)), 5, cv::Scalar(0,255,0), 2);
        }
        // cv::imshow("OpenCV Debug View (Original with Detections)", displayFrame);
    }
}
