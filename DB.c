//DB API-PARKING


/*Sistema donde podra haber mas de un parqueadero*/

/*informacion del parqueadero*/
CREATE TABLE parqueadero(
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL,
  direccion VARCHAR(255) NOT NULL, 
  telefono VARCHAR(20),
  email VARCHAR(255),
  codigo VARCHAR(255), 
  legal LONGTEXT
);

/*Roles del sistema*/
//Super Admin
//Admin
//Usuario
CREATE TABLE rol(
    id INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100)
)

CREATE TABLE usuario(
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL,
  apellido VARCHAR(100) NOT NULL,
  cedula VARCHAR(20) UNIQUE NOT NULL,
  telefono VARCHAR(20),
  email VARCHAR(100) UNIQUE,
  password VARCHAR(255),
  rol_id INT NOT NULL,
  parqueadero_id INT NOT NULL,
  FOREIGN KEY (rol_id) REFERENCES rol(id),
  FOREIGN KEY (parqueadero_id) REFERENCES parqueadero(id)
);

/*CARRO, CAMIONETA, MOTO, CICLA, etc*/
CREATE TABLE tipos_vehiculo (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL 
);

/*Se definen las tarifas por parqueadero*/
CREATE TABLE tarifas (
  id INT AUTO_INCREMENT PRIMARY KEY,
  parqueadero_id INT NOT NULL,
  tipo_vehiculo_id INT NOT NULL,
  tarifa_dia DECIMAL(10,2) NOT NULL,
  tarifa_mes DECIMAL(10,2) NOT NULL,
  FOREIGN KEY (parqueadero_id) REFERENCES parqueaderos(id),
  FOREIGN KEY (tipo_vehiculo_id) REFERENCES tipos_vehiculo(id),
  UNIQUE (parqueadero_id, tipo_vehiculo_id)
);

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

/*si se implementa pasarelas de pagos*/
CREATE TABLE metodos_pago (
  id INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(50) NOT NULL
);
