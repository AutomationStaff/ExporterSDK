/************************************************* *
** Copyright(c) 2021 -- Camshaft Software PTY LTD
************************************************** */


#include "stdafx.h"


void AuExpMesh::SaveMeshFile(const AuCarExpMesh* mesh, const wchar_t* filename)
{

	//save textures:
	for (unsigned int i = 0; i < mesh->GetIndexBufferCount(); i++)
	{
		AuExpManager::Instance()->AddImage(mesh->GetMaterial(i)->GetDiffuseMapData().Texture);
		AuExpManager::Instance()->AddImage(mesh->GetMaterial(i)->GetNormalMapData().Texture);
	}

	//save the mesh:

	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, filename, L"wt");

	if (file)
	{
		unsigned int vertexCount = mesh->GetVertexCount();
		AuCarExpVertex* vertexBuffer = mesh->GetVertexBuffer();

		//positions:
		for (unsigned int i = 0; i < vertexCount; i++)
		{
			fwprintf_s(file, L"v %f %f %f\n", vertexBuffer[i].Position.x, vertexBuffer[i].Position.y, vertexBuffer[i].Position.z);
		}

		//UVs:
		for (unsigned int i = 0; i < vertexCount; i++)
		{
			fwprintf_s(file, L"vt %f %f %f\n", vertexBuffer[i].Position.x, vertexBuffer[i].Position.y, vertexBuffer[i].Position.z);
		}

		//normals
		for (unsigned int i = 0; i < vertexCount; i++)
		{
			fwprintf_s(file, L"vn %f %f %f\n", vertexBuffer[i].Position.x, vertexBuffer[i].Position.y, vertexBuffer[i].Position.z);
		}

		//triangles:
		unsigned int indexBufferCount = mesh->GetIndexBufferCount();

		for (unsigned int i = 0; i < indexBufferCount; i++)
		{
			unsigned int indexCount = mesh->GetIndexCount(i);
			int* indexBuffer = mesh->GetIndexBuffer(i);

			for (unsigned int j = 0; j < indexCount; j += 3)
			{
				int index0 = indexBuffer[j] + 1;
				int index1 = indexBuffer[j + 1] + 1;
				int index2 = indexBuffer[j + 2] + 1;

				fwprintf_s(file, L"f %i/%i/%i %i/%i/%i %i/%i/%i\n", index0, index0, index0, index1, index1, index1, index2, index2, index2);
			}
		}

		fclose(file);
	}
}