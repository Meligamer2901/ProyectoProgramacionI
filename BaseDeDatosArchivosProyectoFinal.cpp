#include <iostream>
#include <string>
#include <vector> 
#include <mysql.h>
#include <iomanip> 
#include <algorithm>
using namespace std;

class MySQLConnector {
private:
    MYSQL* connection;
    MYSQL_RES* res;
    MYSQL_STMT* stmt;

public:
    MySQLConnector(const char* host, const char* user, const char* password, const char* database, int port) {
        connection = mysql_init(0);
        connection = mysql_real_connect(connection, host, user, password, database, port, NULL, 0);
        if (connection) {
            cout << "Conexion Exitosa..." << endl;
        }
        else {
            cout << "Error en la Conexion...";
        }
    }

    void insert(const string& query) {
        executeQuery(query);
    }

    void update(const string& query) {
        executeQuery(query);
    }

    void del(const string& query) {
        executeQuery(query);
    }

    bool select(const string& query) {
        if (mysql_query(connection, query.c_str()) == 0) {
            res = mysql_store_result(connection);
            if (res) {
                int num_fields = mysql_num_fields(res);
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                vector<size_t> max_lengths(num_fields, 0); // Longitud m�xima de cada campo

                // Calcular la longitud m�xima de cada campo
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL) {
                    for (int i = 0; i < num_fields; i++) {
                        if (row[i] && strlen(row[i]) > max_lengths[i]) {
                            max_lengths[i] = strlen(row[i]);
                        }
                    }
                }

                // Imprimir los datos con alineaci�n
                mysql_data_seek(res, 0); // Reiniciar el cursor
                while ((row = mysql_fetch_row(res)) != NULL) {
                    for (int i = 0; i < num_fields; i++) {
                        cout << left << setw(max_lengths[i] + 2) << row[i];
                    }
                    cout << endl;
                }

                return true; // Se encontraron resultados
            }
        }
        else {
            cout << "Error al ejecutar el query: " << mysql_error(connection) << endl;
        }

        return false; // No se encontraron resultados o hubo un error
    }


    ~MySQLConnector() {
        mysql_free_result(res);
        mysql_close(connection);
    }

private:
    void executeQuery(const string& query) {
        if (mysql_query(connection, query.c_str()) == 0) {
            cout << "Query ejecutado correctamente." << endl;
        }
        else {
            cout << "Error al ejecutar el query: " << mysql_error(connection) << endl;
        }
    }
};

// Prototipos de las funciones
void updateClientes(MySQLConnector& connector, const string& tabla);
void updateEmpleados(MySQLConnector& connector, const string& tabla);
void updateProductos(MySQLConnector& connector, const string& tabla);
void updateVentas(MySQLConnector& connector, const string& tabla);
void updateDetallesVentas(MySQLConnector& connector, const string& tabla);
void insertarEnEmpleados();
void insertarEnProductos();
void insertarEnClientes();
void insertarEnVenta();
void insertarEnDetallesVenta();

int main() {
    MySQLConnector connector("localhost", "root", "/BBCN1978*1973", "proyectoProgra", 3306);

    int opcion;
    string query; // Solo declara la variable aqu�, sin inicializarla

    do {
        cout << "\nMenu:\n1. Insertar registro\n2. Actualizar registro\n3. Eliminar registro\n4. Mostrar registros\n5. Salir\nOpcion: ";
        cin >> opcion;

        string tabla;
        switch (opcion) {
        case 1:
        {
            cout << "Seleccione la tabla en la que desea insertar datos:\n1. Empleados\n2. Productos\n3. Clientes\n4. Venta\n5. Detalles_Venta\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;

            switch (opcionTabla) {
            case 1:
                tabla = "Empleados";
                insertarEnEmpleados();
                break;
            case 2:
                tabla = "Productos";
                insertarEnProductos();
                break;
            case 3:
                tabla = "Clientes";
                insertarEnClientes();
                break;
            case 4:
                tabla = "Venta";
                insertarEnVenta();
                break;
            case 5:
                tabla = "Detalles_Venta";
                insertarEnDetallesVenta();
                break;
            default:
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }

            break;
        }
        case 2:
        {
            cout << "Seleccione la tabla que desea modificar:\n1. Empleados\n2. Productos\n3. Clientes\n4. Ventas\n5. Detalles_Venta\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;

            switch (opcionTabla) {
            case 1:
                tabla = "Empleados";
                updateEmpleados(connector, tabla);
                break;
            case 2:
                tabla = "Productos";
                updateProductos(connector, tabla);
                break;
            case 3:
                tabla = "Clientes";
                updateClientes(connector, tabla);
                break;
            case 4:
                tabla = "Ordenes";
                updateVentas(connector, tabla);
                break;
            case 5:
                tabla = "Detalles_Orden";
                updateDetallesVentas(connector, tabla);
                break;
            default:
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }
            break;
        }
        case 3:
        {
            cout << "Seleccione la tabla que contiene el objeto que desea eliminar:\n1. Clientes\n2. Detalle_Venta\n3. Empleados\n4. Productos\n5. Ventas\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;

            string tabla;
            switch (opcionTabla) {
            case 1:
                tabla = "clientes";
                break;
            case 2:
                tabla = "detalle_venta";
                break;
            case 3:
                tabla = "empleados";
                break;
            case 4:
                tabla = "productos";
                break;
            case 5:
                tabla = "ventas";
                break;
            default:
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }

            string id;
            cout << "Ingrese el ID o NIT del registro que desea eliminar de la tabla " << tabla << ": ";
            cin.ignore();
            getline(cin, id);

            // Verificar si el ID pertenece a la tabla seleccionada
            string query = "SELECT id FROM " + tabla + " WHERE id = '" + id + "'";
            if (connector.select(query)) {
                // El ID pertenece a la tabla, proceder con la eliminaci�n
                query = "DELETE FROM " + tabla + " WHERE id = '" + id + "'";
                connector.del(query);
            }
            else {
                cout << "El ID especificado no pertenece a la tabla seleccionada." << endl;
            }

            break;
        }

        case 4:
        {
            cout << "Seleccione la tabla que desea visualizar:\n1. Clientes\n2. Detalle_Venta\n3. Empleados\n4. Productos\n5. Ventas\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;

            switch (opcionTabla) {
            case 1:
                tabla = "clientes";
                break;
            case 2:
                tabla = "detalle_venta";
                break;
            case 3:
                tabla = "empleados";
                break;
            case 4:
                tabla = "productos";
                break;
            case 5:
                tabla = "ventas";
                break;
            default:
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }

            cout << "Mostrando todos los registros de la tabla " << tabla << ":" << endl;
            query = "SELECT * FROM " + tabla;
            connector.select(query);
            break;
        }
        case 5:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion no valida. Por favor, intente de nuevo." << endl;
        }
    } while (opcion != 5);

    cout << "Presione Enter para salir...";
    cin.ignore();
    cin.get(); // Esperar a que el usuario presione Enter

    return 0;
}


void updateClientes(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el NIT del cliente que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. NIT\n2. Nombre\n3. Apellido\n4. Email\n5. Telefono\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {
    case 1:
        campo = "NIT cliente";
        break;
    case 2:
        campo = "Nombre del cliente";
        break;
    case 3:
        campo = "Apellido del cliente";
        break;
    case 4:
        campo = "Email";
        break;
    case 5:
        campo = "Numero Telefonico";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id = " + to_string(id);
    connector.update(query);
}
void updateEmpleados(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID del empleado que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Nombre\n2. Apellido\n3. Puesto\n4. Sexo\n5. Salario\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {
    case 1:
        campo = "Nombre del empleado";
        break;
    case 2:
        campo = "Apellido del empleado";
        break;
    case 3:
        campo = "Puesto del trabajador";
        break;
    case 4:
        campo = "Ingrese el sexo (M/F)";
        break;
    case 5:
        campo = "Salario laboral";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id = " + to_string(id);
    connector.update(query);
}

void updateProductos(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID del producto que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Nombre del producto\n2. Tipo de producto\n3. Fecha\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {

    case 1:
        campo = "Nombre del producto";
        break;
    case 2:
        campo = "Tipo de producto";
        break;
    case 3:
        campo = "Fecha de caducidad";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id = " + to_string(id);
    connector.update(query);
}

void updateVentas(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID de la venta que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Fecha\n2. Total de venta\n3. NIT del cliente\n4. ID de empleado\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {
    case 1:
        campo = "Fecha";
        break;
    case 2:
        campo = "Total de venta";
        break;
    case 3:
        campo = "El NIT del cliente";
        break;
    case 4:
        campo = "ID de empleado";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id = " + to_string(id);
    connector.update(query);
}

void updateDetallesVentas(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID del detalle de venta que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Cantidad\n2. Precio\n3. ID_Producto\n4. ID_Venta\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {
    case 1:
        campo = "Cantidad";
        break;
    case 2:
        campo = "Precio";
        break;
    case 3:
        campo = "ID del producto";
        break;
    case 4:
        campo = "ID de la venta";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id = " + to_string(id);
    connector.update(query);
}
void insertarEnEmpleados() {
    string nombre, apellido, puesto, salario, sexo;
    cout << "Nombre del empleado: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Apellido del empleado: ";
    cin.ignore();
    getline(cin, apellido);
    cout << "Puesto del trabajador: ";
    cin.ignore();
    getline(cin, puesto);
    cout << "Sexo (M/F): ";
    cin >> sexo;
    transform(sexo.begin(), sexo.end(), sexo.begin(), ::toupper);
    while (sexo != "M" && sexo != "F") {
        cout << "Entrada inv�lida. Por favor ingrese 'M' para Masculino o 'F' para Femenino: ";
        cin >> sexo;
        transform(sexo.begin(), sexo.end(), sexo.begin(), ::toupper); 
    }
    cout << "Cantidad de salario: ";
    cin.ignore();
    getline(cin, salario);

    string query = "INSERT INTO Empleados (nombre, apellido, puesto, sexo, salario) VALUES ('" + nombre + "','" + apellido + "','" + puesto + "','" + sexo + "','" + salario + "')";
    cout << query << endl; 
}

void insertarEnProductos() {
    string nombre, descripcion, fecha;
    cout << "Nombre del producto: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Tipo de producto: ";
    cin.ignore();
    getline(cin, descripcion);
    cout << "Fecha de caducidad (Ano-mes-dia): ";
    cin.ignore();
    getline(cin, fecha);

    string query = "INSERT INTO Productos (nombre, descripcion, fecha_caducidad) VALUES ('" + nombre + "', '" + descripcion + "', '" + fecha + "')";
    // connector.insert(query); // Descomentar esta l�nea cuando tengas el objeto connector configurado
    cout << query << endl; // Para demostraci�n
}

void insertarEnClientes() {
    string nit, nombre, apellido, telefono, email;
    cout << "Ingrese el NIT: ";
    cin.ignore();
    getline(cin, nit);
    cout << "Nombre del cliente: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Apellido del cliente: ";
    cin.ignore();
    getline(cin, apellido);
    cout << "Email: ";
    cin.ignore();
    getline(cin, email);
    cout << "Numero de telefono: ";
    cin.ignore();
    getline(cin, telefono);

    string query = "INSERT INTO Clientes (nit, nombre, apellido, email, telefono) VALUES ('" + nit + "','" + nombre + "','" + apellido + "','" + email + "','" + telefono + "')";
    // connector.insert(query); // Descomentar esta l�nea cuando tengas el objeto connector configurado
    cout << query << endl; // Para demostraci�n
}

void insertarEnVenta() {
    int empleado_id;
    string cliente_id, fecha;
    double total;
    cout << "NIT del cliente: ";
    cin.ignore();
    getline(cin, cliente_id);
    cout << "ID del empleado: ";
    cin >> empleado_id;
    cout << "Fecha de Compra: ";
    cin.ignore();
    getline(cin, fecha);
    cout << "Total de la venta: ";
    cin >> total;

    string query = "INSERT INTO Venta (fecha, total, cliente_id, empleado_id) VALUES ('" + fecha + "', " + to_string(total) + ",'" + cliente_id + "', " + to_string(empleado_id) + ")";
    // connector.insert(query); // Descomentar esta l�nea cuando tengas el objeto connector configurado
    cout << query << endl; // Para demostraci�n
}

void insertarEnDetallesVenta() {
    int venta_id, producto_id, cantidad;
    double precio;
    cout << "ID de venta: ";
    cin >> venta_id;
    cout << "ID de producto: ";
    cin >> producto_id;
    cout << "Cantidad del producto: ";
    cin >> cantidad;
    cout << "Precio del producto: ";
    cin >> precio;

    string query = "INSERT INTO Detalles_Orden (cantidad, precio, producto_id, venta_id) VALUES (" + to_string(cantidad) + "," + to_string(precio) + ", " + to_string(producto_id) + ", " + to_string(venta_id) + ")";
    // connector.insert(query); // Descomentar esta l�nea cuando tengas el objeto connector configurado
    cout << query << endl; // Para demostraci�n
}