# DataBase_Algorithm_IR
 # Motor Mínimo de Base de Datos

## Descripción del Proyecto

Este proyecto consiste en el desarrollo de un motor de base de datos mínimo implementado en lenguaje C. El objetivo es construir un sistema que pueda procesar y ejecutar comandos SQL básicos, cumpliendo con las funciones fundamentales de creación, manipulación y consulta de datos de manera eficiente y estructurada.

## Objetivos

- Implementar un motor de base de datos mínimo.
- Entender la arquitectura y comunicación de componentes de un sistema de bases de datos.
- Aplicar buenas prácticas de programación y patrones de diseño en C.

## Componentes

1. **Interfaz REPL (Read-Eval-Print Loop)**: Recibe las instrucciones para generar tablas y operar con ellas.
2. **Tokenizador**: Divide el texto SQL en tokens y entrega esos tokens al analizador.
3. **Analizador Sintáctico (Parser)**: Reconoce comandos del SQL básico.
4. **Generador de Código**: Genera bytecode a partir del árbol de análisis.
5. **Motor de Bytecode / Máquina Virtual**: Ejecuta el bytecode generado.
6. **Skip List**: Indexa la base de datos para operaciones rápidas.

## Requerimientos de Usuario

- Interacción mediante una interfaz de línea de comandos que acepte comandos SQL básicos.
- Soporte para comandos SQL como `CREATE DATABASE`, `CREATE TABLE`, `INSERT INTO`, `SELECT`, `WHERE`, `LIMIT`, `FETCH`, y `CASE`.

## Requerimientos de Sistema

- Sistema de caché de páginas con tamaño configurable.
- Uso de listas Skip List para indexación.
- Compatibilidad con el sistema operativo Linux.
- Inclusión de pruebas unitarias e integración.

## Requerimientos Funcionales

1. **Interfaz de Línea de Comandos (REPL)**: Procesa comandos SQL en tiempo real.
2. **Interpretación de Comandos SQL**: Tokenizer, Parser y Generador de Código.
3. **Ejecución de Comandos SQL**: Crear bases de datos, tablas, insertar y consultar datos.
4. **Ejecución del Bytecode**: Utiliza una máquina virtual interna.
5. **Indexación de Datos**: Implementación de Skip List.
6. **Manejo de Páginas y Caché**: Lectura y escritura de datos en páginas de tamaño fijo.
7. **Interfaz de Entrada/Salida con el Sistema Operativo**: Lectura y escritura de datos en archivos.
8. **Pruebas Unitarias e Integración**: Verificación del correcto funcionamiento de cada módulo.
9. **Utilidades de Mantenimiento y Depuración**: Herramientas para revisar el estado de la base de datos.
10. **Gestión de Errores**: Manejo de errores de entrada de usuario y mensajes claros.

## Requerimientos No Funcionales

- **Rendimiento**: Tiempo de respuesta razonable para operaciones básicas.
- **Escalabilidad**: Diseñado para funcionar con una cantidad de datos pequeña a mediana.
- **Portabilidad**: Compatibilidad con Linux.
- **Confiabilidad y Mantenimiento**: Pruebas unitarias e integración.
- **Usabilidad**: Interfaz REPL intuitiva y mensajes informativos.
- **Consumo de Recursos**: Ligero en consumo de memoria y almacenamiento en disco.
- **Limitaciones de Concurrencia**: No soporta múltiples usuarios simultáneos.
- **Extensibilidad**: Arquitectura modular para futuras mejoras.
- **Seguridad Básica**: No incluye medidas avanzadas de seguridad.
- **Compatibilidad con Conjuntos de Caracteres y Codificación**: Soporte solo para caracteres ASCII.

## Arquitectura General

- **REPL Interface**: Procesa la entrada de comandos.
- **Tokenizer**: Convierte el texto SQL en tokens.
- **Parser**: Analiza la sintaxis de los tokens.
- **Code Generator**: Genera bytecode.
- **Bytecode Engine/Virtual Machine**: Ejecuta el bytecode.
- **Skip List**: Maneja la indexación.
- **Page Cache**: Administra la lectura, escritura y caché de las páginas de datos.
- **Interface to OS/IO**: Interactúa con el sistema operativo para acceso a archivos.

## Estructura del Proyecto

motor_db/
├── include/        # Archivos de encabezado
├── src/            # Código fuente
├── tests/          # Pruebas unitarias
├── Makefile        # Archivo para compilar el proyecto
└── README.md       # Documentación del proyecto

## Pruebas Unitarias

El proyecto incluye pruebas unitarias para verificar la correcta creación de tablas, inserción de datos y selección/filtrado de elementos.

## Conclusiones

El desarrollo de este motor de base de datos mínimo en C ha permitido comprender de manera práctica el funcionamiento de los componentes esenciales de un sistema de bases de datos, aplicando conceptos fundamentales y avanzados de bases de datos, compiladores y sistemas operativos.

## Referencias

- SQLite Architecture
