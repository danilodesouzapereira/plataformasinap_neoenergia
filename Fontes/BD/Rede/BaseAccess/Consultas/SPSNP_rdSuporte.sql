  SELECT
    Suporte.intSuporteId          AS SuporteId,
    Suporte.intTipoSuporteId      AS TipoSuporteId,
    Suporte.strDescricao          AS Descricao,
    Suporte.strExtId              AS ExtId,
    Suporte.fltVmin               AS Vmin,
    Suporte.fltVmax               AS Vmax,
    Suporte.intNumPontosFix       AS NumPontosFix,
    Suporte.intStatus             AS Status,
    SuporteFixacao.intFixacaoItem AS FixacaoItem,
    SuporteFixacao.fltX           AS FixacaoX,
    SuporteFixacao.fltY           AS FixacaoY,
    SuporteFixacao.strCodigo      AS FixacaoCodigo
  FROM
    Suporte INNER JOIN SuporteFixacao ON
      Suporte.intSuporteId = SuporteFixacao.intSuporteId
  WHERE
    (( [p_SuporteId] = -1) OR (Suporte.intSuporteId = [p_SuporteId] ))
  ORDER BY
    Suporte.intSuporteId;
