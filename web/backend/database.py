import os
from datetime import datetime
from typing import Optional
from sqlalchemy import create_engine, Column, String, Integer, DateTime, Text, Boolean
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
