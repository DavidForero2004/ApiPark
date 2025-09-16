-- DB API-PARKING
CREATE DATABASE IF NOT EXISTS parkingAPI;
USE parkingAPI;

-- 1. Informacion del parqueadero
CREATE TABLE parqueadero (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL,
  direccion VARCHAR(255) NOT NULL, 
  telefono VARCHAR(20),
  email VARCHAR(255),
  codigo VARCHAR(255), 
  legal LONGTEXT,
  UNIQUE(nombre, direccion)
);

-- 2. Roles del sistema
-- Super Admin, Admin, Usuario
CREATE TABLE rol (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL
);

-- 3. Usuario
CREATE TABLE usuario (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL,
  apellido VARCHAR(100) NOT NULL,
  cedula VARCHAR(20) NOT NULL,
  telefono VARCHAR(20),
  email VARCHAR(100),
  password VARCHAR(255),
  rol_id INT NOT NULL,
  parqueadero_id INT NOT NULL,
  FOREIGN KEY (rol_id) REFERENCES rol(id),
  FOREIGN KEY (parqueadero_id) REFERENCES parqueadero(id),
  UNIQUE(cedula, email, parqueadero_id )
);

-- 4. Tipos de vehiculo
-- Ej: Carro, Camioneta, Moto, Bici, etc.
CREATE TABLE tipos_vehiculo (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL 
);

-- 5. Tarifas por parqueadero
CREATE TABLE tarifas (
  id INT AUTO_INCREMENT PRIMARY KEY,
  parqueadero_id INT NOT NULL,
  tipo_vehiculo_id INT NOT NULL,
  tarifa_dia DECIMAL(10,2) NOT NULL,
  tarifa_mes DECIMAL(10,2) NOT NULL,
  FOREIGN KEY (parqueadero_id) REFERENCES parqueadero(id),
  FOREIGN KEY (tipo_vehiculo_id) REFERENCES tipos_vehiculo(id),
  UNIQUE (parqueadero_id, tipo_vehiculo_id)
);

-- 6. Mensualidades
CREATE TABLE mensualidad (
  id INT AUTO_INCREMENT PRIMARY KEY,
  usuario_id INT NOT NULL,             -- Cliente
  tipo_vehiculo_id INT NOT NULL,       -- Tipo de vehículo
  placa VARCHAR(20) NOT NULL,         
  tarifa_id INT NOT NULL,              -- Tarifa aplicada
  fecha_inicio DATE NOT NULL,
  fecha_fin DATE NOT NULL,
  estado ENUM('ACTIVA','VENCIDA','CANCELADA') DEFAULT 'ACTIVA',
  parqueadero_id INT NOT NULL,         -- Parqueadero donde aplica
  FOREIGN KEY (usuario_id) REFERENCES usuario(id),
  FOREIGN KEY (tipo_vehiculo_id) REFERENCES tipos_vehiculo(id),
  FOREIGN KEY (tarifa_id) REFERENCES tarifas(id),
  FOREIGN KEY (parqueadero_id) REFERENCES parqueadero(id),
  UNIQUE (parqueadero_id, placa) -- una placa no puede repetirse en el mismo parqueadero
);

-- 7. Transacciones
CREATE TABLE transacciones (
  id INT AUTO_INCREMENT PRIMARY KEY,
  usuario_id INT NOT NULL,               -- Cliente afectado
  parqueadero_id INT NOT NULL,           -- Parqueadero donde ocurrió
  mensualidad_id INT NOT NULL,           -- Relación directa a la mensualidad
  tipo ENUM('CREACION','RENOVACION','PAGO','CANCELACION') NOT NULL,
  monto DECIMAL(10,2) DEFAULT 0.00,      -- Valor del movimiento
  descripcion MEDIUMTEXT,                -- Detalles de la transacción
  fecha DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (usuario_id) REFERENCES usuario(id),
  FOREIGN KEY (parqueadero_id) REFERENCES parqueadero(id),
  FOREIGN KEY (mensualidad_id) REFERENCES mensualidad(id)
);

-- 8. Metodos de pago
CREATE TABLE metodos_pago (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(50) NOT NULL
);

-- 9. Tabla hash (para guardar códigos)
CREATE TABLE hash (
  id INT AUTO_INCREMENT PRIMARY KEY,
  code LONGTEXT NOT NULL
);


-- CONSULTAR HASH

DELIMITER //

CREATE PROCEDURE hash_Validate(IN p_code LONGTEXT)
BEGIN
    SELECT CASE 
        WHEN EXISTS (SELECT 1 FROM hash WHERE code = p_code)
        THEN 1 ELSE 0 
    END AS valido;
END //

DELIMITER ;

-- Crear procedimiento para insertar uno o varios roles
DELIMITER //
CREATE PROCEDURE sp_createRol(IN p_nombre VARCHAR(100))
BEGIN
    INSERT INTO rol (nombre) VALUES (p_nombre);
END //
DELIMITER ;

-- Obtener todos los roles
DELIMITER //
CREATE PROCEDURE sp_getRoles()
BEGIN
    SELECT * FROM rol;
END //
DELIMITER ;

-- Obtener rol por id
DELIMITER //
CREATE PROCEDURE sp_getRolById(IN p_id INT)
BEGIN
    SELECT * FROM rol WHERE id = p_id;
END //
DELIMITER ;

-- Actualizar rol
DELIMITER //
CREATE PROCEDURE sp_updateRol(IN p_id INT, IN p_nombre VARCHAR(100))
BEGIN
    UPDATE rol SET nombre = p_nombre WHERE id = p_id;
END //
DELIMITER ;

-- Eliminar rol
DELIMITER //
CREATE PROCEDURE sp_deleteRol(IN p_id INT)
BEGIN
    DELETE FROM rol WHERE id = p_id;
END //
DELIMITER ;


-- Crear parqueadero
DELIMITER //
CREATE PROCEDURE sp_createParqueadero(
    IN p_nombre VARCHAR(100),
    IN p_direccion VARCHAR(255),
    IN p_telefono VARCHAR(20),
    IN p_email VARCHAR(255),
    IN p_codigo VARCHAR(255),
    IN p_legal LONGTEXT
)
BEGIN
    INSERT INTO parqueadero (nombre, direccion, telefono, email, codigo, legal)
    VALUES (p_nombre, p_direccion, p_telefono, p_email, p_codigo, p_legal);
END //
DELIMITER ;

-- Obtener todos los parqueaderos
DELIMITER //
CREATE PROCEDURE sp_getParqueaderos()
BEGIN
    SELECT * FROM parqueadero;
END //
DELIMITER ;

-- Obtener parqueadero por id
DELIMITER //
CREATE PROCEDURE sp_getParqueaderoById(IN p_id INT)
BEGIN
    SELECT * FROM parqueadero WHERE id = p_id;
END //
DELIMITER ;

-- Actualizar parqueadero
DELIMITER //
CREATE PROCEDURE sp_updateParqueadero(
    IN p_id INT,
    IN p_nombre VARCHAR(100),
    IN p_direccion VARCHAR(255),
    IN p_telefono VARCHAR(20),
    IN p_email VARCHAR(255),
    IN p_codigo VARCHAR(255),
    IN p_legal LONGTEXT
)
BEGIN
    UPDATE parqueadero
    SET nombre = p_nombre,
        direccion = p_direccion,
        telefono = p_telefono,
        email = p_email,
        codigo = p_codigo,
        legal = p_legal
    WHERE id = p_id;
END //
DELIMITER ;

-- Eliminar parqueadero
DELIMITER //
CREATE PROCEDURE sp_deleteParqueadero(IN p_id INT)
BEGIN
    DELETE FROM parqueadero WHERE id = p_id;
END //
DELIMITER ;


-- Crear usuario
DELIMITER //
CREATE PROCEDURE sp_createUsuario(
  IN p_nombre VARCHAR(100),
  IN p_apellido VARCHAR(100),
  IN p_cedula VARCHAR(20),
  IN p_telefono VARCHAR(20),
  IN p_email VARCHAR(100),
  IN p_password VARCHAR(255),
  IN p_rol_id INT,
  IN p_parqueadero_id INT
)
BEGIN
  INSERT INTO usuario (nombre, apellido, cedula, telefono, email, password, rol_id, parqueadero_id)
  VALUES (p_nombre, p_apellido, p_cedula, p_telefono, p_email, p_password, p_rol_id, p_parqueadero_id);
END //
DELIMITER ;

-- Obtener todos los usuarios
DELIMITER //
CREATE PROCEDURE sp_getUsuarios()
BEGIN
  SELECT * FROM usuario;
END //
DELIMITER ;

-- Obtener usuario por ID
DELIMITER //
CREATE PROCEDURE sp_getUsuarioById(IN p_id INT)
BEGIN
  SELECT * FROM usuario WHERE id = p_id;
END //
DELIMITER ;

-- Actualizar usuario
DELIMITER //
CREATE PROCEDURE sp_updateUsuario(
  IN p_id INT,
  IN p_nombre VARCHAR(100),
  IN p_apellido VARCHAR(100),
  IN p_cedula VARCHAR(20),
  IN p_telefono VARCHAR(20),
  IN p_email VARCHAR(100),
  IN p_password VARCHAR(255),
  IN p_rol_id INT,
  IN p_parqueadero_id INT
)
BEGIN
  UPDATE usuario
  SET nombre = p_nombre,
      apellido = p_apellido,
      cedula = p_cedula,
      telefono = p_telefono,
      email = p_email,
      password = p_password,
      rol_id = p_rol_id,
      parqueadero_id = p_parqueadero_id
  WHERE id = p_id;
END //
DELIMITER ;

-- Eliminar usuario
DELIMITER //
CREATE PROCEDURE sp_deleteUsuario(IN p_id INT)
BEGIN
  DELETE FROM usuario WHERE id = p_id;
END //
DELIMITER ;


-- Crear tipo de vehículo
DELIMITER //
CREATE PROCEDURE sp_createTipoVehiculo(IN p_nombre VARCHAR(100))
BEGIN
    INSERT INTO tipos_vehiculo (nombre) VALUES (p_nombre);
END //
DELIMITER ;

-- Obtener todos los tipos de vehículo
DELIMITER //
CREATE PROCEDURE sp_getTiposVehiculo()
BEGIN
    SELECT * FROM tipos_vehiculo;
END //
DELIMITER ;

-- Obtener tipo por id
DELIMITER //
CREATE PROCEDURE sp_getTipoVehiculoById(IN p_id INT)
BEGIN
    SELECT * FROM tipos_vehiculo WHERE id = p_id;
END //
DELIMITER ;

-- Actualizar tipo
DELIMITER //
CREATE PROCEDURE sp_updateTipoVehiculo(IN p_id INT, IN p_nombre VARCHAR(100))
BEGIN
    UPDATE tipos_vehiculo SET nombre = p_nombre WHERE id = p_id;
END //
DELIMITER ;

-- Eliminar tipo
DELIMITER //
CREATE PROCEDURE sp_deleteTipoVehiculo(IN p_id INT)
BEGIN
    DELETE FROM tipos_vehiculo WHERE id = p_id;
END //
DELIMITER ;
