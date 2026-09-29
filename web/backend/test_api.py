"""
Basic API endpoint tests for JOCKY backend
"""
import pytest
from fastapi.testclient import TestClient
from app import app


@pytest.fixture
def client():
    return TestClient(app)


def test_config_endpoint(client):
    response = client.get("/api/config")
    assert response.status_code == 200
    data = response.json()
    assert "api_version" in data
    assert "supported_platforms" in data
    assert "obfuscation_presets" in data


def test_runtime_apis_endpoint(client):
    response = client.get("/api/runtime-apis")
    assert response.status_code == 200
    assert isinstance(response.json(), list)


def test_obfuscation_passes_endpoint(client):
    response = client.get("/api/obfuscation-passes")
    assert response.status_code == 200
    data = response.json()
    assert "mlir" in data
    assert "llvm" in data


def test_demo_script_windows(client):
    response = client.get("/api/demo-script/windows")
    assert response.status_code == 200
    data = response.json()
    assert data["platform"] == "windows"
    assert "source" in data


def test_demo_script_linux(client):
    response = client.get("/api/demo-script/linux")
    assert response.status_code == 200
    data = response.json()
    assert data["platform"] == "linux"
    assert "source" in data


def test_demo_script_invalid_platform(client):
    response = client.get("/api/demo-script/macos")
    assert response.status_code == 400


def test_compile_request_validation(client):
    """Test that compilation request validation works"""
    response = client.post(
        "/api/compile",
        json={
            "source": "",
            "platform": "windows",
        },
    )
    assert response.status_code == 422


def test_compile_request_invalid_platform(client):
    """Test invalid platform rejection"""
    response = client.post(
        "/api/compile",
        json={
            "source": "fn main() {}",
            "platform": "macos",
        },
    )
    assert response.status_code == 422


def test_compile_request_invalid_preset(client):
    """Test invalid preset rejection"""
    response = client.post(
        "/api/compile",
        json={
            "source": "fn main() {}",
            "platform": "windows",
            "preset": "extreme",
        },
    )
    assert response.status_code == 422


def test_examples_endpoint(client):
    response = client.get("/api/examples")
    assert response.status_code == 200
    data = response.json()
    assert "examples" in data


def test_examples_by_platform(client):
    response = client.get("/api/examples?platform=windows")
    assert response.status_code == 200
    data = response.json()
    assert "examples" in data


def test_validate_source_endpoint(client):
    response = client.post(
        "/api/validate-source",
        json={
            "source": "fn main() {}",
            "platform": "windows",
        },
    )
    assert response.status_code == 200
    data = response.json()
    assert "errors" in data
    assert "has_errors" in data
