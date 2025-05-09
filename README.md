# 🕺🎮 Motion Transfer 2D sin Deep Learning

Este proyecto busca implementar un sistema de **Motion Transfer** en 2D 🎥➡️🧍‍♂️ sin el uso de técnicas de aprendizaje profundo. En su lugar, se emplean técnicas clásicas de visión por computadora para capturar el movimiento de una persona y transferirlo a un personaje animado en 2D mediante la librería **SFML** 🖼️.

---

## 🔍 Descripción

🎯 **Objetivo general**: Capturar los movimientos de una persona en tiempo real y transferirlos a un modelo animado 2D utilizando métodos tradicionales de procesamiento de imagen.

### 🛠️ Tecnologías y herramientas

- 🎨 **Segmentación por color** para detección inicial de puntos clave del cuerpo.
- 📈 En el futuro se reemplazará por un estimador de N puntos más robusto (ej: algoritmo geométrico, keypoint detector no basado en redes).
- 🧩 **SFML (Simple and Fast Multimedia Library)** para la visualización del personaje animado.
- 🧍‍♀️ El personaje 2D tendrá un mapeo directo a los puntos de la persona para imitar sus movimientos.

---

## 📁 Estructura del Proyecto

```
motion-transfer-2d/
├── src/                 # Código fuente
├── assets/              # Sprites y recursos gráficos
├── data/                # Datos de prueba o grabaciones
├── README.md            # Este archivo
└── ...
```

---

## 🚀 Cómo ejecutar el proyecto

1. 🧰 **Requisitos previos**:
   - [ ] Instalar SFML: [https://www.sfml-dev.org/](https://www.sfml-dev.org/)
   - [ ] Compilador C++ compatible (ej. `g++`)
   - [ ] CMake (opcional, si usas build automático)

2. ⚙️ **Compilación**:

```bash
# Clona este repositorio
git clone https://github.com/usuario/proyecto-motion-transfer.git
cd proyecto-motion-transfer

# Compila el proyecto
g++ -std=c++17 src/main.cpp -o motion_transfer -lsfml-graphics -lsfml-window -lsfml-system

# Ejecuta el programa
./motion_transfer
```

3. 🧪 **Uso**:
   - Asegúrate de tener una cámara disponible y luz suficiente para que la segmentación por color funcione correctamente.
   - El personaje debería comenzar a replicar tus movimientos en tiempo real en la ventana de SFML.

---

## 🧠 Estado del desarrollo

- [x] Segmentación por color funcional 🎨
- [ ] Implementación de estimación de múltiples puntos N 🤖
- [ ] Sistema de configuración de personajes 2D con articulaciones
- [ ] Documentación técnica completa 📄

---

## 👨‍👩‍👧‍👦 Integrantes

| Nombre | GitHub | Rol |
|--------|--------|-----|
| ✍️ Nombre 1 | [@usuario1](https://github.com/usuario1) | - |
| 💡 Nombre 2 | [@usuario2](https://github.com/usuario2) | - |
| 🎨 Nombre 3 | [@usuario3](https://github.com/usuario3) | - |
| 🧪 Nombre 4 | [@usuario4](https://github.com/usuario4) | - |

---

## 📌 Notas

- Este proyecto es parte de una exploración académica/prototipo y puede tener limitaciones.
- Se busca una solución completamente libre de aprendizaje profundo para comprender mejor los principios clásicos del motion tracking.

---

## 📷 Capturas (opcional)

_Agrega aquí gifs o imágenes del sistema en funcionamiento._

---

## 📝 Licencia

Este proyecto está licenciado bajo [MIT](LICENSE).

---
