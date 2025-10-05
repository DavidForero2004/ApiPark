-- DB API-PARKING
CREATE DATABASE IF NOT EXISTS parkingAPI;
USE parkingAPI;

-- 1. Parqueadero
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

-- 2. Roles
CREATE TABLE rol (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL
);

-- 3. Usuarios
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
  UNIQUE(cedula, email, parqueadero_id)
);

-- 4. Tipos de vehículo
CREATE TABLE tipos_vehiculo (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL
);

-- 5. Tarifas
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
  usuario_id INT NOT NULL,
  tarifa_id INT NOT NULL,
  placa VARCHAR(20) NOT NULL,
  fecha_inicio DATE NOT NULL,
  fecha_fin DATE NOT NULL,
  estado ENUM('ACTIVA','VENCIDA','CANCELADA') DEFAULT 'ACTIVA',
  FOREIGN KEY (usuario_id) REFERENCES usuario(id),
  FOREIGN KEY (tarifa_id) REFERENCES tarifas(id),
  UNIQUE(usuario_id, placa)
);

-- 7. Transacciones
CREATE TABLE transacciones (
  id INT AUTO_INCREMENT PRIMARY KEY,
  usuario_id INT NOT NULL,
  mensualidad_id INT NOT NULL,
  tipo ENUM('CREACION','RENOVACION','PAGO','CANCELACION') NOT NULL,
  monto DECIMAL(10,2) DEFAULT 0.00,
  descripcion MEDIUMTEXT,
  fecha DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (usuario_id) REFERENCES usuario(id),
  FOREIGN KEY (mensualidad_id) REFERENCES mensualidad(id)
);

-- 8. Métodos de pago
CREATE TABLE metodos_pago (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(50) NOT NULL
);

-- 9. Hash
CREATE TABLE hash (
  id INT AUTO_INCREMENT PRIMARY KEY,
  code LONGTEXT NOT NULL
);

--PROCEDURES
DELIMITER //

-- HASH
CREATE PROCEDURE hash_Validate(IN p_code LONGTEXT)
BEGIN
    SELECT CASE WHEN EXISTS (SELECT 1 FROM hash WHERE code = p_code) THEN 1 ELSE 0 END AS valido;
END //

-- ROLES
CREATE PROCEDURE sp_createRol(IN p_nombre VARCHAR(100)) BEGIN INSERT INTO rol(nombre) VALUES(p_nombre); END //
CREATE PROCEDURE sp_getRoles() BEGIN SELECT * FROM rol; END //
CREATE PROCEDURE sp_getRolById(IN p_id INT) BEGIN SELECT * FROM rol WHERE id=p_id; END //
CREATE PROCEDURE sp_updateRol(IN p_id INT, IN p_nombre VARCHAR(100)) BEGIN UPDATE rol SET nombre=p_nombre WHERE id=p_id; END //
CREATE PROCEDURE sp_deleteRol(IN p_id INT) BEGIN DELETE FROM rol WHERE id=p_id; END //

-- PARQUEADERO
CREATE PROCEDURE sp_createParqueadero(IN p_nombre VARCHAR(100), IN p_direccion VARCHAR(255), IN p_telefono VARCHAR(20), IN p_email VARCHAR(255), IN p_codigo VARCHAR(255), IN p_legal LONGTEXT)
BEGIN INSERT INTO parqueadero(nombre,direccion,telefono,email,codigo,legal) VALUES(p_nombre,p_direccion,p_telefono,p_email,p_codigo,p_legal); END //
CREATE PROCEDURE sp_getParqueaderos() BEGIN SELECT * FROM parqueadero; END //
CREATE PROCEDURE sp_getParqueaderoById(IN p_id INT) BEGIN SELECT * FROM parqueadero WHERE id=p_id; END //
CREATE PROCEDURE sp_updateParqueadero(IN p_id INT, IN p_nombre VARCHAR(100), IN p_direccion VARCHAR(255), IN p_telefono VARCHAR(20), IN p_email VARCHAR(255), IN p_codigo VARCHAR(255), IN p_legal LONGTEXT)
BEGIN UPDATE parqueadero SET nombre=p_nombre,direccion=p_direccion,telefono=p_telefono,email=p_email,codigo=p_codigo,legal=p_legal WHERE id=p_id; END //
CREATE PROCEDURE sp_deleteParqueadero(IN p_id INT) BEGIN DELETE FROM parqueadero WHERE id=p_id; END //

-- USUARIO
CREATE PROCEDURE sp_createUsuario(IN p_nombre VARCHAR(100), IN p_apellido VARCHAR(100), IN p_cedula VARCHAR(20), IN p_telefono VARCHAR(20), IN p_email VARCHAR(100), IN p_password VARCHAR(255), IN p_rol_id INT, IN p_parqueadero_id INT)
BEGIN INSERT INTO usuario(nombre,apellido,cedula,telefono,email,password,rol_id,parqueadero_id) VALUES(p_nombre,p_apellido,p_cedula,p_telefono,p_email,p_password,p_rol_id,p_parqueadero_id); END //

DELIMITER //
CREATE PROCEDURE sp_getUsuarios(IN p_parqueadero_id INT)
BEGIN
    IF p_parqueadero_id = 0 THEN
        SELECT u.id,
               u.nombre,
               u.apellido,
               u.cedula,
               u.telefono,
               u.email,
               u.password,
               u.rol_id,
               u.parqueadero_id as parking_id,
               p.nombre AS parqueadero_nombre
        FROM usuario u
        INNER JOIN parqueadero p ON u.parqueadero_id = p.id;
    ELSE
        SELECT u.id,
               u.nombre,
               u.apellido,
               u.cedula,
               u.telefono,
               u.email,
               u.password,
               u.rol_id,
               u.parqueadero_id as parking_id,
               p.nombre AS parqueadero_nombre
        FROM usuario u
        INNER JOIN parqueadero p ON u.parqueadero_id = p.id
        WHERE u.parqueadero_id = p_parqueadero_id;
    END IF;
END //
DELIMITER ;


CREATE PROCEDURE sp_getUsuarioById(IN p_id INT) BEGIN SELECT * FROM usuario WHERE id=p_id; END //
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
    SET 
        nombre         = COALESCE(p_nombre, nombre),
        apellido       = COALESCE(p_apellido, apellido),
        cedula         = COALESCE(p_cedula, cedula),
        telefono       = COALESCE(p_telefono, telefono),
        email          = COALESCE(p_email, email),
        password       = COALESCE(p_password, password),
        rol_id         = COALESCE(p_rol_id, rol_id),
        parqueadero_id = COALESCE(p_parqueadero_id, parqueadero_id)
    WHERE id = p_id;
END //

DELIMITER ;

-- TIPOS DE VEHÍCULO
CREATE PROCEDURE sp_createTipoVehiculo(IN p_nombre VARCHAR(100)) BEGIN INSERT INTO tipos_vehiculo(nombre) VALUES(p_nombre); END //
CREATE PROCEDURE sp_getTiposVehiculo() BEGIN SELECT * FROM tipos_vehiculo; END //
CREATE PROCEDURE sp_getTipoVehiculoById(IN p_id INT) BEGIN SELECT * FROM tipos_vehiculo WHERE id=p_id; END //
CREATE PROCEDURE sp_updateTipoVehiculo(IN p_id INT, IN p_nombre VARCHAR(100)) BEGIN UPDATE tipos_vehiculo SET nombre=p_nombre WHERE id=p_id; END //
CREATE PROCEDURE sp_deleteTipoVehiculo(IN p_id INT) BEGIN DELETE FROM tipos_vehiculo WHERE id=p_id; END //

-- TARIFAS
CREATE PROCEDURE sp_createTarifa(IN p_parqueadero_id INT, IN p_tipo_vehiculo_id INT, IN p_tarifa_dia DECIMAL(10,2), IN p_tarifa_mes DECIMAL(10,2))
BEGIN INSERT INTO tarifas(parqueadero_id,tipo_vehiculo_id,tarifa_dia,tarifa_mes) VALUES(p_parqueadero_id,p_tipo_vehiculo_id,p_tarifa_dia,p_tarifa_mes); END //

CREATE PROCEDURE sp_getTarifas(IN p_parqueadero_id INT)
BEGIN
    IF p_parqueadero_id = 0 THEN
        SELECT * FROM tarifas;
    ELSE
        SELECT * FROM tarifas WHERE parqueadero_id = p_parqueadero_id;
    END IF;
END //

CREATE PROCEDURE sp_getTarifaById(IN p_id INT) BEGIN SELECT * FROM tarifas WHERE id=p_id; END //
CREATE PROCEDURE sp_updateTarifa(IN p_id INT, IN p_parqueadero_id INT, IN p_tipo_vehiculo_id INT, IN p_tarifa_dia DECIMAL(10,2), IN p_tarifa_mes DECIMAL(10,2))
BEGIN UPDATE tarifas SET parqueadero_id=p_parqueadero_id,tipo_vehiculo_id=p_tipo_vehiculo_id,tarifa_dia=p_tarifa_dia,tarifa_mes=p_tarifa_mes WHERE id=p_id; END //
CREATE PROCEDURE sp_deleteTarifa(IN p_id INT) BEGIN DELETE FROM tarifas WHERE id=p_id; END //

-- MENSUALIDADES
CREATE PROCEDURE sp_createMensualidad(IN p_usuario_id INT, IN p_tarifa_id INT, IN p_placa VARCHAR(20), IN p_fecha_inicio DATE, IN p_fecha_fin DATE, IN p_estado ENUM('ACTIVA','VENCIDA','CANCELADA'))
BEGIN INSERT INTO mensualidad(usuario_id,tarifa_id,placa,fecha_inicio,fecha_fin,estado) VALUES(p_usuario_id,p_tarifa_id,p_placa,p_fecha_inicio,p_fecha_fin,p_estado); END //

CREATE PROCEDURE sp_getMensualidades(IN p_parqueadero_id INT)
BEGIN
    IF p_parqueadero_id = 0 THEN
        SELECT m.* FROM mensualidad m;
    ELSE
        SELECT m.* 
        FROM mensualidad m
        INNER JOIN usuario u ON m.usuario_id = u.id
        WHERE u.parqueadero_id = p_parqueadero_id;
    END IF;
END //

CREATE PROCEDURE sp_getMensualidadById(IN p_id INT) BEGIN SELECT * FROM mensualidad WHERE id=p_id; END //
CREATE PROCEDURE sp_updateMensualidad(IN p_id INT, IN p_usuario_id INT, IN p_tarifa_id INT, IN p_placa VARCHAR(20), IN p_fecha_inicio DATE, IN p_fecha_fin DATE, IN p_estado ENUM('ACTIVA','VENCIDA','CANCELADA'))
BEGIN UPDATE mensualidad SET usuario_id=p_usuario_id,tarifa_id=p_tarifa_id,placa=p_placa,fecha_inicio=p_fecha_inicio,fecha_fin=p_fecha_fin,estado=p_estado WHERE id=p_id; END //
CREATE PROCEDURE sp_deleteMensualidad(IN p_id INT) BEGIN DELETE FROM mensualidad WHERE id=p_id; END //

-- TRANSACCIONES
CREATE PROCEDURE sp_createTransaccion(IN p_usuario_id INT, IN p_mensualidad_id INT, IN p_tipo ENUM('CREACION','RENOVACION','PAGO','CANCELACION'), IN p_monto DECIMAL(10,2), IN p_descripcion MEDIUMTEXT)
BEGIN INSERT INTO transacciones(usuario_id,mensualidad_id,tipo,monto,descripcion) VALUES(p_usuario_id,p_mensualidad_id,p_tipo,p_monto,p_descripcion); END //

CREATE PROCEDURE sp_getTransacciones(IN p_parqueadero_id INT)
BEGIN
    IF p_parqueadero_id = 0 THEN
        SELECT t.* FROM transacciones t;
    ELSE
        SELECT t.*
        FROM transacciones t
        INNER JOIN usuario u ON t.usuario_id = u.id
        WHERE u.parqueadero_id = p_parqueadero_id;
    END IF;
END //

CREATE PROCEDURE sp_getTransaccionById(IN p_id INT) BEGIN SELECT * FROM transacciones WHERE id=p_id; END //
CREATE PROCEDURE sp_updateTransaccion(IN p_id INT, IN p_usuario_id INT, IN p_mensualidad_id INT, IN p_tipo ENUM('CREACION','RENOVACION','PAGO','CANCELACION'), IN p_monto DECIMAL(10,2), IN p_descripcion MEDIUMTEXT)
BEGIN UPDATE transacciones SET usuario_id=p_usuario_id,mensualidad_id=p_mensualidad_id,tipo=p_tipo,monto=p_monto,descripcion=p_descripcion WHERE id=p_id; END //
CREATE PROCEDURE sp_deleteTransaccion(IN p_id INT) BEGIN DELETE FROM transacciones WHERE id=p_id; END //

DELIMITER ;

-- LOGIN
DELIMITER //
CREATE PROCEDURE sp_login_usuario(IN p_email VARCHAR(255))
BEGIN
      SELECT u.id, u.nombre, u.email, u.password, r.id AS rol, u.parqueadero_id
    FROM usuario u
    INNER JOIN rol r ON u.rol_id = r.id
    WHERE u.email = p_email
    LIMIT 1;
END //
DELIMITER ;



--PROOF DATA

-- ROLES
INSERT INTO rol(nombre) VALUES 
('Super Admin'),
('Admin'),
('Usuario');

-- PARQUEADEROS
INSERT INTO parqueadero(nombre,direccion,telefono,email,codigo,legal) VALUES
('Parqueadero Central','Calle 100 #10-20','3001234567','central@parking.com','PC001','Legalidad ok'),
('Parqueadero Norte','Carrera 15 #50-30','3009876543','norte@parking.com','PN002','Legalidad ok');

-- USUARIOS
INSERT INTO usuario(nombre,apellido,cedula,telefono,email,password,rol_id,parqueadero_id) VALUES
('Juan','Perez','1234567890','3001112233','juanp@gmail.com','12345',1,1),
('Maria','Lopez','0987654321','3004445566','marial@gmail.com','12345',2,2);

-- TIPOS DE VEHÍCULO
INSERT INTO tipos_vehiculo(nombre) VALUES
('Carro'),
('Moto'),
('Camioneta');

-- TARIFAS
INSERT INTO tarifas(parqueadero_id,tipo_vehiculo_id,tarifa_dia,tarifa_mes) VALUES
(1,1,5000,120000),
(1,2,3000,70000),
(2,1,6000,130000),
(2,3,8000,150000);

-- MENSUALIDADES
INSERT INTO mensualidad(usuario_id,tarifa_id,placa,fecha_inicio,fecha_fin,estado) VALUES
(1,1,'ABC123', '2025-09-01', '2025-09-30', 'ACTIVA'),
(2,3,'XYZ987', '2025-09-01', '2025-09-30', 'ACTIVA');

-- TRANSACCIONES
INSERT INTO transacciones(usuario_id,mensualidad_id,tipo,monto,descripcion) VALUES
(1,1,'CREACION',120000,'Mensualidad septiembre 2025'),
(2,2,'CREACION',150000,'Mensualidad septiembre 2025');

-- METODOS DE PAGO
INSERT INTO metodos_pago(nombre) VALUES
('Efectivo'),
('Tarjeta Debito'),
('Tarjeta Crédito'),
('Transferencia');

-- HASH
INSERT INTO hash(code) VALUES
('ABC123HASH'),
('XYZ987HASH');

