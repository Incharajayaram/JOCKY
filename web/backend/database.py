import os
from datetime import datetime
from typing import Optional
from sqlalchemy import create_engine, Column, String, Integer, DateTime, Text, Boolean, Float
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker, Session

DATABASE_URL = os.getenv("DATABASE_URL", "sqlite:///./jocky_jobs.db")
engine = create_engine(DATABASE_URL, connect_args={"check_same_thread": False} if "sqlite" in DATABASE_URL else {})
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
Base = declarative_base()


class JobRecord(Base):
    __tablename__ = "jobs"

    id = Column(String, primary_key=True, index=True)
    status = Column(String, default="pending")
    platform = Column(String, default="windows")
    created_at = Column(DateTime, default=datetime.utcnow)
    completed_at = Column(DateTime, nullable=True)
    progress = Column(Integer, default=0)
    output_path = Column(String, nullable=True)
    output_name = Column(String, nullable=True)
    logs = Column(Text, default="")
    error_message = Column(String, nullable=True)
    compilation_time = Column(Integer, nullable=True)


class TelemetryEvent(Base):
    __tablename__ = "telemetry_events"

    id = Column(Integer, primary_key=True, index=True)
    timestamp = Column(String, index=True)
    chain_id = Column(String, index=True)
    event_type = Column(String, index=True)
    phase = Column(Integer, nullable=True)
    status = Column(String, nullable=True)
    details_json = Column(Text, nullable=True)
    received_at = Column(DateTime, default=datetime.utcnow, index=True)


def get_db() -> Session:
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


def init_db():
    Base.metadata.create_all(bind=engine)


def save_job(db: Session, job_id: str, status: str, platform: str, progress: int, logs: str, error_message: Optional[str] = None):
    record = db.query(JobRecord).filter(JobRecord.id == job_id).first()
    if not record:
        record = JobRecord(id=job_id, status=status, platform=platform, progress=progress, logs=logs, error_message=error_message)
        db.add(record)
    else:
        record.status = status
        record.platform = platform
        record.progress = progress
        record.logs = logs
        record.error_message = error_message
        if status in ("completed", "failed"):
            record.completed_at = datetime.utcnow()

    db.commit()
    return record


def get_job_record(db: Session, job_id: str) -> Optional[JobRecord]:
    return db.query(JobRecord).filter(JobRecord.id == job_id).first()


def list_jobs(db: Session, limit: int = 50, offset: int = 0):
    return db.query(JobRecord).order_by(JobRecord.created_at.desc()).limit(limit).offset(offset).all()


def save_telemetry_event(db: Session, chain_id: str, timestamp: str, event_type: str, phase: Optional[int] = None, status: Optional[str] = None, details_json: Optional[str] = None) -> TelemetryEvent:
    event = TelemetryEvent(
        chain_id=chain_id,
        timestamp=timestamp,
        event_type=event_type,
        phase=phase,
        status=status,
        details_json=details_json,
    )
    db.add(event)
    db.commit()
    db.refresh(event)
    return event


def get_telemetry_events(db: Session, chain_id: str, limit: int = 50) -> list[TelemetryEvent]:
    return db.query(TelemetryEvent).filter(TelemetryEvent.chain_id == chain_id).order_by(TelemetryEvent.received_at.desc()).limit(limit).all()


def get_telemetry_events_range(db: Session, chain_id: str, start_timestamp: str = None, end_timestamp: str = None) -> list[TelemetryEvent]:
    query = db.query(TelemetryEvent).filter(TelemetryEvent.chain_id == chain_id)
    if start_timestamp:
        query = query.filter(TelemetryEvent.timestamp >= start_timestamp)
    if end_timestamp:
        query = query.filter(TelemetryEvent.timestamp <= end_timestamp)
    return query.order_by(TelemetryEvent.received_at).all()


def cleanup_old_telemetry(db: Session, hours: int = 24) -> int:
    cutoff = datetime.utcnow().replace(microsecond=0) - __import__('datetime').timedelta(hours=hours)
    deleted = db.query(TelemetryEvent).filter(TelemetryEvent.received_at < cutoff).delete()
    db.commit()
    return deleted
